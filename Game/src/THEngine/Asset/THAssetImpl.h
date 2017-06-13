#ifndef TH_ASSET_IMPL_H
#define TH_ASSET_IMPL_H

#include <Common\THCommon.h>

namespace THEngine
{
	class AssetImpl : public Object
	{
	protected:
		AssetImpl();
		virtual ~AssetImpl();

		virtual void OnLostDevice() = 0;
		virtual void OnResetDevice() = 0;
	};
}

#endif