#ifndef TH_ASSET_IMPL_H
#define TH_ASSET_IMPL_H

#include <Common\THCommon.h>

namespace THEngine
{
	class BaseAsset : public Object
	{
	protected:
		BaseAsset() = default;
		virtual ~BaseAsset() = default;

		virtual void OnLostDevice() = 0;
		virtual void OnResetDevice() = 0;
	};
}

#endif