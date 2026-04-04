#pragma once

#include "HevcEncoder.g.h"
#include "mfidl.h"
#include "mfapi.h"
#include "mftransform.h"

namespace winrt::MagicBsod_Codecs_RT::implementation
{
	struct HevcEncoder : HevcEncoderT<HevcEncoder, IMFTransform, IMFMediaEventGenerator, IMFShutdown>
	{
		HevcEncoder();
		static void Register();
		static void Register(Windows::Foundation::Collections::IPropertySet configuration);
		static void ResetAttempts();
		static UINT32 GetNumEncoders();
		static winrt::hstring GetSelectedCodec();
		void PopulateHEVCAvailableCodecs(winrt::hresult& hr, IMFActivate** activationObjs, std::vector<int>& codecList, std::map<unsigned int, std::string>& codecMap);

		void SetProperties(Windows::Foundation::Collections::IPropertySet configuration);

		HRESULT __stdcall GetStreamLimits(DWORD* pdwInputMinimum, DWORD* pdwInputMaximum, DWORD* pdwOutputMinimum, DWORD* pdwOutputMaximum) noexcept override;
		HRESULT __stdcall GetStreamCount(DWORD* pcInputStreams, DWORD* pcOutputStreams) noexcept override;
		HRESULT __stdcall GetStreamIDs(DWORD dwInputIDArraySize, DWORD* pdwInputIDs, DWORD dwOutputIDArraySize, DWORD* pdwOutputIDs) noexcept override;
		HRESULT __stdcall GetInputStreamInfo(DWORD dwInputStreamID, MFT_INPUT_STREAM_INFO* pStreamInfo) noexcept override;
		HRESULT __stdcall GetOutputStreamInfo(DWORD dwOutputStreamID, MFT_OUTPUT_STREAM_INFO* pStreamInfo) noexcept override;
		HRESULT __stdcall GetAttributes(IMFAttributes** pAttributes) noexcept override;
		HRESULT __stdcall GetInputStreamAttributes(DWORD dwInputStreamID, IMFAttributes** pAttributes) noexcept override;
		HRESULT __stdcall GetOutputStreamAttributes(DWORD dwOutputStreamID, IMFAttributes** pAttributes) noexcept override;
		HRESULT __stdcall DeleteInputStream(DWORD dwStreamID) noexcept override;
		HRESULT __stdcall AddInputStreams(DWORD cStreams, DWORD* adwStreamIDs) noexcept override;
		HRESULT __stdcall GetInputAvailableType(DWORD dwInputStreamID, DWORD dwTypeIndex, IMFMediaType** ppType) noexcept override;
		HRESULT __stdcall GetOutputAvailableType(DWORD dwOutputStreamID, DWORD dwTypeIndex, IMFMediaType** ppType) noexcept override;
		HRESULT __stdcall SetInputType(DWORD dwInputStreamID, IMFMediaType* pType, DWORD dwFlags) noexcept override;
		HRESULT __stdcall SetOutputType(DWORD dwOutputStreamID, IMFMediaType* pType, DWORD dwFlags) noexcept override;
		HRESULT __stdcall GetInputCurrentType(DWORD dwInputStreamID, IMFMediaType** ppType) noexcept override;
		HRESULT __stdcall GetOutputCurrentType(DWORD dwOutputStreamID, IMFMediaType** ppType) noexcept override;
		HRESULT __stdcall GetInputStatus(DWORD dwInputStreamID, DWORD* pdwFlags) noexcept override;
		HRESULT __stdcall GetOutputStatus(DWORD* pdwFlags) noexcept override;
		HRESULT __stdcall SetOutputBounds(LONGLONG hnsLowerBound, LONGLONG hnsUpperBound) noexcept override;
		HRESULT __stdcall ProcessEvent(DWORD dwInputStreamID, IMFMediaEvent* pEvent) noexcept override;
		HRESULT __stdcall ProcessMessage(MFT_MESSAGE_TYPE eMessage, ULONG_PTR ulParam) noexcept override;
		HRESULT __stdcall ProcessInput(DWORD dwInputStreamID, IMFSample* pSample, DWORD dwFlags) noexcept override;
		HRESULT __stdcall ProcessOutput(DWORD dwFlags, DWORD cOutputBufferCount, MFT_OUTPUT_DATA_BUFFER* pOutputSamples, DWORD* pdwStatus) noexcept override;

		HRESULT __stdcall GetEvent(DWORD dwFlags, IMFMediaEvent** ppEvent) noexcept override;
		HRESULT __stdcall BeginGetEvent(IMFAsyncCallback* pCallback, IUnknown* punkState) noexcept override;
		HRESULT __stdcall EndGetEvent(IMFAsyncResult* pResult, IMFMediaEvent** ppEvent) noexcept override;
		HRESULT __stdcall QueueEvent(MediaEventType met, REFGUID guidExtendedType, HRESULT hrStatus, const PROPVARIANT* pvValue) noexcept override;

		HRESULT __stdcall Shutdown() noexcept override;
		HRESULT __stdcall GetShutdownStatus(MFSHUTDOWN_STATUS* pStatus) noexcept override;

	private:
		com_ptr<IMFTransform> m_wrappedEncoder;
		com_ptr<IMFMediaEventGenerator> m_mediaEventGenerator;
		com_ptr<IMFShutdown> m_shutdown;
	};
}

namespace winrt::MagicBsod_Codecs_RT::factory_implementation
{
	struct HevcEncoder : HevcEncoderT<HevcEncoder, implementation::HevcEncoder>
	{
	};
}
