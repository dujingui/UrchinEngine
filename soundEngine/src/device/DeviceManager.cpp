#include <AL/al.h>
#include <AL/alc.h>
#include <stdexcept>

#include "DeviceManager.h"

namespace urchin
{

	DeviceManager::DeviceManager()
	{

	}

	DeviceManager::~DeviceManager()
	{

	}

	/**
	 * Initialize OpenAL: create context and make it current
	 */
	void DeviceManager::initializeDevice()
	{
	    ALCdevice *device = alcOpenDevice(nullptr);
	    if (!device)
	    {
	        throw std::runtime_error("Impossible to found sound device.");
	    }

	    ALCcontext *context = alcCreateContext(device, nullptr);
	    if (!context)
	    {
	    	throw std::runtime_error("Impossible to create sound context.");
	    }

	    if (!alcMakeContextCurrent(context))
	    {
	    	throw std::runtime_error("Impossible to make context current.");
	    }
	}

	/**
	 * Shutdown OpenAL: destroy context and close device
	 */
	void DeviceManager::shutdownDevice()
	{
	    ALCcontext *context = alcGetCurrentContext();
	    ALCdevice *device  = alcGetContextsDevice(context);

	    alcMakeContextCurrent(nullptr);
	    alcDestroyContext(context);

	    alcCloseDevice(device);
	}

}
