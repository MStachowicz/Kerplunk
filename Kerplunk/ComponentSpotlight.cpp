#include "ComponentSpotlight.h"



ComponentSpotlight::ComponentSpotlight() : IComponent(IComponent::ComponentFlags::COMPONENT_SPOTLIGHT),
position(DEFAULT_SPOT_POSITION),
direction(DEFAULT_SPOT_DIRECTION),
ambient(DEFAULT_SPOT_COLOUR), diffuse(DEFAULT_SPOT_COLOUR),
specular(DEFAULT_SPOT_SPECULAR),
constant(DEFAULT_SPOT_CONSTANT_ATTENUATION),
linear(DEFAULT_SPOT_LINEAR_ATTENUATION),
quadratic(DEFAULT_SPOT_QUADRATIC_ATTENUATION),
innerCutOff(DEFAULT_SPOT_INNER_CUTOFF),
outerCutOff(DEFAULT_SPOT_OUTER_CUTOFF)
{
}


ComponentSpotlight::~ComponentSpotlight()
{
}
