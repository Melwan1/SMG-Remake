#include "stage.hpp"

class LumaStage : public Stage
{
    public:

        LumaStage(int requirements);

        int get_requirements() const;

    private:
        
        int _requirements;
};