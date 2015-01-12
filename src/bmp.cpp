#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "bmp.hpp"

#define BITMAP_MAGIC_NUMBER 19778


// http://www.cplusplus.com/forum/general/74215/
// http://stackoverflow.com/questions/8836017/reading-a-bmp-file-in-c
#pragma pack(2) // Add this

struct BitmapHeader {
	uint16_t id;
	uint32_t fileSize;
	uint16_t reserved1, reserved2;
	uint32_t offset;
};

#pragma pack()

struct BitmapInfoHeader {
	uint32_t headerSize;
	uint32_t width, height;
	uint16_t planes, bits;
	uint32_t compressed;
	uint32_t imageSize;			
	int32_t  Xdpi;				/* Horizontal pixels per meter */
	int32_t  Ydpi;				/* Vertical pixels per meter */
    uint32_t colors;        	/* Number of colors used */
    uint32_t importantColors;   /* Number of important colors */	
};

std::unique_ptr<double[]> charge_bmp256(const std::string& fichier, uint32_t& largeur, uint32_t& hauteur) {
	std::ifstream image(fichier, std::ios::binary);
	if(!image || !image.is_open())
	{
		std::cerr << "Impossible d'ouvrir l'image " << fichier << std::endl;
		return nullptr;
	}
	BitmapHeader header = {0};
	image.read((char*)&header,sizeof(BitmapHeader));
	BitmapInfoHeader infos = {0};
	image.read((char*)&infos, sizeof(BitmapInfoHeader));
	hauteur = infos.height;
	largeur = infos.width;
		
	std::unique_ptr<uint8_t[]> pixels(new uint8_t[largeur*hauteur]);
	image.seekg(header.offset, std::ios_base::beg);
	image.read((char*)pixels.get(), sizeof(uint8_t)*largeur*hauteur);

	std::unique_ptr<double[]> m(new double[largeur*hauteur]);
	for (uint32_t y = 0; y < hauteur; y++) {
		for (uint32_t x = 0; x < largeur; x++) {
			m[x + largeur * (hauteur - 1 - y)] = (double) pixels[x + largeur
					* y];
		}
	}

	return std::move(m);
}

bool ecrit_bmp256(const std::string& fichier, uint32_t largeur, uint32_t hauteur, double* m) {
	std::ofstream image(fichier, std::ios::binary);
	if(!image || !image.is_open())
	{
		std::cerr << "Unable to open file " << fichier << " for writing" << std::endl;
		return false;
	}
	// Conversion double => uint8_t
	std::unique_ptr<uint8_t[]> pixels(new uint8_t[largeur * hauteur]);
	for (uint32_t y = 0; y < hauteur; y++) {
		for (uint32_t x = 0; x < largeur; x++) {
			double d;
			uint8_t c;
			d = m[x + largeur * y];
			if (d < 0.0)
				c = 0;
			else if (d > 255.0)
				c = 255;
			else
				c = (uint8_t) d;

			pixels[x + largeur * (hauteur - 1 - y)] = c;
		}
	}

	BitmapHeader header = {0};
	header.id = BITMAP_MAGIC_NUMBER;
	header.fileSize = largeur * hauteur + 14 + 40 + 256*4; // taille image + taille BITMAPFILEHEADER + taille BITMAPINFOHEADER + taille palette
	header.offset = 14+40+256*4; // taille BITMAPFILEHEADER + taille BITMAPINFOHEADER + taille palette
	BitmapInfoHeader infos = {0};
	infos.headerSize = 40;
	infos.width = largeur;
	infos.height = hauteur;
	infos.planes = 1;
	infos.bits = 8;
	infos.compressed = 0;
	infos.Xdpi = 0;
	infos.Ydpi = 0;
	infos.importantColors = 0;
	
	image.write((char*)&header, sizeof(BitmapHeader));
	image.write((char*)&infos, sizeof(BitmapInfoHeader));

	// Ecriture de la palette en niveaux de gris
	for (int i = 0; i < 256; i++) {
		unsigned char uc = static_cast<unsigned char>(i);
		for(int j = 0; j<4; ++j) 
			image.write((char*)&uc, 1);
	}

	image.write((char*)pixels.get(),largeur*hauteur*sizeof(char));
	return true;
}