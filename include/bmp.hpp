extern "C" {
	double* charge_bmp256(char* fichier, uint32_t* largeur, uint32_t* hauteur);
	int ecrit_bmp256(char* fichier, uint32_t largeur, uint32_t hauteur, double* m);
}