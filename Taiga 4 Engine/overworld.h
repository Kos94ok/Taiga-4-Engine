
class cOverworldMap
{
	sf::Vector2f pos;
	std::string name;
	std::string descr;
};

class cOverworld
{
public:
	int activeMap;
	std::vector<cOverworldMap> map;

	void generateMap();
	//void save(std::string saveName);
	//void load(std::string saveName);
};