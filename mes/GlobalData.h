#include <string>

class GlobalData {

    public:
        int elementsCount;
        int nodesCount;
        int length;
        int surface;
        int k;
        
        GlobalData();
        ~GlobalData();

        bool loadFromFile(std::string path);
};
