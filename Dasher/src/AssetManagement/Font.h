#include "Maths.h"
#include <vector>

extern bool FontInit();
extern void FontCleanUp();

struct FontCharacter
{
	FontCharacter() = default;
	FontCharacter(const FontCharacter&) = default;
	FontCharacter(unsigned int tex, const glm::ivec2& size, const glm::ivec2& bearing, unsigned int advance);

	unsigned int texId;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

class Font
{
public:
	Font() = default;
	Font(const Font&) = default;

	inline const FontCharacter& GetFontChar(int index) const { return m_vCharacters.at(index); }
	bool LoadFont(const char* const path, unsigned int height);

	void Cleanup();
private:
	std::vector<FontCharacter> m_vCharacters;
};

