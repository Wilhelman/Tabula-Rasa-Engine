#ifndef __RESOURCE_ANIMATION_H__
#define __RESOURCE_ANIMATION_H__

#include "Resource.h"
#include "MathGeoLib/Math/float4x4.h"

#include <vector>

class ResourceAnimation : public Resource
{

public:
	struct BoneTransformation
	{
		std::string bone_name;


		struct Key
		{
			enum KeyType {
				POSITION = 0,
				SCALE,
				ROTATION,

				UNKNOWN
			};

			void Init(KeyType type, uint count) {

				time = new double[count];

				switch (type)
				{
				case ResourceAnimation::BoneTransformation::Key::POSITION:
				case ResourceAnimation::BoneTransformation::Key::SCALE:
					value = new float[3 * count];
					break;
				case ResourceAnimation::BoneTransformation::Key::ROTATION:
					value = new float[4 * count];
					break;
				case ResourceAnimation::BoneTransformation::Key::UNKNOWN:
					break;
				default:
					break;
				}
			}

			~Key()
			{
				RELEASE_ARRAY(time);
				RELEASE_ARRAY(value);
			}

			uint count = 0;
			double* time = nullptr;
			float* value = nullptr;
		};

		Key position;
		Key scale;
		Key rotation;
	};

public:
	ResourceAnimation(UID uid);
	~ResourceAnimation();

	bool LoadInMemory() override;
	bool ReleaseMemory() override;

public:
	std::string name;
	double duration;
	double ticks_per_second;

	

	uint num_keys = 0;
	std::vector<BoneTransformation*> bone_keys;

};

#endif // __RESOURCE_ANIMATION_H__