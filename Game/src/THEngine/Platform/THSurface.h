#ifndef TH_SURFACE_H
#define TH_SURFACE_H

#include <Common\THCommon.h>

namespace THEngine
{
	class Surface : public Object
	{
	private:
		IDirect3DSurface9* surface = nullptr;

	public:
		virtual ~Surface();

		bool SaveToFile(const String& path);

		friend class Device;
	};
}

#endif
