// Transform header file to define the interface. 
// The class is all static for simplicity

// Include the helper glm library, including matrix transform extensions

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// glm provides vector, matrix classes like glsl
// Typedefs to make code more readable 

typedef glm::mat3 mat3;
typedef glm::mat4 mat4; 
typedef glm::vec3 vec3; 
typedef glm::vec4 vec4; 
const float pi = 3.14159265;

class Transform  
{
public:
        Transform();
        virtual ~Transform();
        
        static mat3 rotate(const float degrees, const vec3& axis);
        static mat4 scale(const float &sx, const float &sy, const float &sz); 
        static mat4 translate(const float &tx, const float &ty, const float &tz);
};