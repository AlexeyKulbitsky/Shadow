#include "Sampler.h"
#include "../Device.h"
#include "Driver.h"

namespace sh
{

namespace video
{

	SamplerPtr Sampler::Create(const SamplerDescription& description)
	{
		Driver* driver = Device::GetInstance()->GetDriver();
		return driver->CreateSampler(description);
	}

} // video

} // sh