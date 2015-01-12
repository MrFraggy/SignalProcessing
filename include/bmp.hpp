#include <cstdint>
#include <memory>

std::unique_ptr<double[]> charge_bmp256(const std::string& fichier, uint32_t& largeur, uint32_t& hauteur);
bool ecrit_bmp256(const std::string& fichier, uint32_t largeur, uint32_t hauteur, double* m);
