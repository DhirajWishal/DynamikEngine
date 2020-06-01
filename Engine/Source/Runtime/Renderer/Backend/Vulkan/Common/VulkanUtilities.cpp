#include "dmkafx.h"
#include "VulkanUtilities.h"

namespace Dynamik
{
	namespace Backend
	{
		VkFormat VulkanUtilities::getVulkanFormat(DMKFormat format)
		{
			return (VkFormat)(UI32)format;
		}

		UI32 VulkanUtilities::findMemoryType(UI32 typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice)
		{
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

			for (UI32 i = 0; i < memProperties.memoryTypeCount; i++)
				if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags
					& properties) == properties)
					return i;

			DMK_ERROR_BOX("Failed to find suitable memory type!");
			return 0;
		}

		B1 VulkanUtilities::hasStencilComponent(const VkFormat& format)
		{
			return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
		}

		VkFormat VulkanUtilities::findSupportedFormat(const ARRAY<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice)
		{
			for (VkFormat format : candidates) {
				VkFormatProperties props;

				vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

				if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
					return format;

				else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
					return format;
			}

			DMK_ERROR_BOX("Failed to find supported format!");
			return VkFormat();
		}

		VkFormat VulkanUtilities::findDepthFormat(const VkPhysicalDevice& physicalDevice)
		{
			return findSupportedFormat(
				{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
				VK_IMAGE_TILING_OPTIMAL,
				VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
				physicalDevice
			);
		}

		VkDescriptorType VulkanUtilities::getDescriptorType(DMKUniformType type)
		{
			switch (type)
			{
			case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_UNIFORM_BUFFER:			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_STORAGE_BUFFER:			return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_UNIFORM_BUFFER_DYNAMIC:	return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_STORAGE_BUFFER_DYNAMIC:	return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
			case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_UNIFORM_TEXEL_BUFFER:	return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
			case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_STORAGE_TEXEL_BUFFER:	return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
			case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_INPUT_ATTACHMENT:		return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
			case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_STORAGE_IMAGE:			return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
			case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_SAMPLER_2D:				return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_SAMPLER_CUBE:			return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_SAMPLER_2D_ARRAY:		return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_SAMPLER_CUBE_ARRAY:		return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			case Dynamik::DMKUniformType::DMK_UNIFORM_TYPE_ACCELERATION_STRUCTURE:	return VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV;
			default:
				DMK_ERROR_BOX("Invalid uniform type!");
				break;
			}

			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		}

		VkShaderStageFlagBits VulkanUtilities::getShaderStage(DMKShaderLocation location)
		{
			switch (location)
			{
			case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX:		return VK_SHADER_STAGE_VERTEX_BIT;
			case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_TESSELLATION:	return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
			case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_GEOMETRY:		return VK_SHADER_STAGE_GEOMETRY_BIT;
			case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_FRAGMENT:		return VK_SHADER_STAGE_FRAGMENT_BIT;
			case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_COMPUTE:		return VK_SHADER_STAGE_COMPUTE_BIT;
			case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_ALL_GRAPHICS:	return VK_SHADER_STAGE_ALL;
			case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_RAY_GEN:		return VK_SHADER_STAGE_RAYGEN_BIT_NV;
			case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_ANY_HIT:		return VK_SHADER_STAGE_ANY_HIT_BIT_NV;
			case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_CLOSEST_HIT:	return VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV;
			case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_MISS:			return VK_SHADER_STAGE_MISS_BIT_NV;
			case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_INTERSECTION:	return VK_SHADER_STAGE_INTERSECTION_BIT_NV;
			case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_CALLABLE:		return VK_SHADER_STAGE_CALLABLE_BIT_NV;
			case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_TASK:			return VK_SHADER_STAGE_TASK_BIT_NV;
			case Dynamik::DMKShaderLocation::DMK_SHADER_LOCATION_MESH:			return VK_SHADER_STAGE_MESH_BIT_NV;
			default:
				DMK_ERROR_BOX("Invalid shader location!");
				break;
			}

			return VK_SHADER_STAGE_ALL;
		}

		ARRAY<VkDescriptorSetLayoutBinding> VulkanUtilities::getDescriptorSetLayoutBindings(ARRAY<DMKUniformBufferDescriptor> descriptors)
		{
			ARRAY<VkDescriptorSetLayoutBinding> _bindings;

			for (auto _descriptor : descriptors)
			{
				for (auto _object : _descriptor.uniformBufferObjects)
				{
					if (_object.type == DMKUniformType::DMK_UNIFORM_TYPE_CONSTANT)
						continue;

					VkDescriptorSetLayoutBinding _binding;
					_binding.binding = _object.destinationBinding;
					_binding.descriptorCount = 1;
					_binding.descriptorType = getDescriptorType(_object.type);
					_binding.pImmutableSamplers = VK_NULL_HANDLE;
					_binding.stageFlags = getShaderStage(_object.shaderLocation);
					_bindings.pushBack(_binding);
				}
			}

			return _bindings;
		}
	}
}