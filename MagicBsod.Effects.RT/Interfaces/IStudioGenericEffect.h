#pragma once
using namespace Windows::Foundation::Collections;
using namespace Windows::Media::Effects;

namespace MagicBsod_Effects_RT::Interfaces
{
	public interface class IStudioGenericEffect
	{
		virtual void SetProperties(IPropertySet^ configuration) = 0;
		virtual void Close(MediaEffectClosedReason reason) = 0;
		virtual void DiscardQueuedFrames() = 0;
	};
}