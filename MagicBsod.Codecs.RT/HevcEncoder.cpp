#include "pch.h"
#include "HevcEncoder.h"
#include "HevcEncoder.g.cpp"
#include <codecapi.h>
#include <icodecapi.h>
#include <winrt/Windows.Media.h>
#include <windows.media.h>

using namespace winrt;
using namespace Windows::Foundation::Collections;

namespace abi {
	using namespace winrt::Windows::Media;
}

static winrt::Windows::Media::MediaExtensionManager mediaExtensionManager;

HRESULT SetAttributeU32(winrt::com_ptr<ICodecAPI> const& codec, const GUID& guid, UINT32 value)
{
    VARIANT val{};
	val.vt = VT_UI4;
	val.uintVal = value;
	return codec->SetValue(&guid, &val);
}


namespace winrt::MagicBsod_Codecs_RT::implementation
{
	UINT32 numHevcEncoders = 0;
	int numHEVCAttempts = 0;
	winrt::hstring codecBeingUsed;

    HevcEncoder::HevcEncoder()
    {
		IMFActivate** activationObjs = nullptr;
		MFT_REGISTER_TYPE_INFO inputType{ MFMediaType_Video, MFVideoFormat_NV12 };
		MFT_REGISTER_TYPE_INFO outputType{ MFMediaType_Video, MFVideoFormat_HEVC };
		//hresult hr = MFTEnumEx(MFT_CATEGORY_VIDEO_ENCODER, MFT_ENUM_FLAG_HARDWARE, &inputType, &outputType, &activationObjs, &numHevcEncoders);
		hresult hr = MFTEnumEx(MFT_CATEGORY_VIDEO_ENCODER, MFT_ENUM_FLAG_ALL, &inputType, &outputType, &activationObjs, &numHevcEncoders);
		check_hresult(hr);

		std::vector<int> codecList;
		std::map<unsigned int, std::string> codecMap;
		PopulateHEVCAvailableCodecs(hr, activationObjs, codecList, codecMap);

        for (int i = numHEVCAttempts; i < codecList.size(); i++)
        {
			hr = activationObjs[codecList[i]]->ActivateObject(IID_PPV_ARGS(m_wrappedEncoder.put()));
			if (FAILED(hr))
            {
                numHEVCAttempts++;
                continue;
            }
            else
            {
				numHEVCAttempts++;
				codecBeingUsed = winrt::to_hstring(codecMap[codecList[i]]);
				break;
            }
        }

		check_hresult(hr);

		for (size_t i = 0; i < numHevcEncoders; i++)
        {
            if (activationObjs[i])
            {
                activationObjs[i]->Release();
            }
        }

		CoTaskMemFree(activationObjs);

        m_wrappedEncoder.try_as(m_mediaEventGenerator);
		m_wrappedEncoder.try_as(m_shutdown);
    }

    void HevcEncoder::PopulateHEVCAvailableCodecs(winrt::hresult& hr, IMFActivate** activationObjs, std::vector<int>& codecList, std::map<unsigned int, std::string>& codecMap)
    {
        for (unsigned int i = 0; i < numHevcEncoders; i++)
        {
            LPWSTR codecName = L"";
            UINT32 nameSize = 0;
			hr = activationObjs[i]->GetAllocatedString(MFT_FRIENDLY_NAME_Attribute, &codecName, &nameSize);
			if (SUCCEEDED(hr))
            {
                std::wstring std_codecName{ L"" };
                if (codecName != 0)
                {
                    std_codecName = codecName;
                }

				codecMap.emplace(i, std::string(std_codecName.begin(), std_codecName.end()));
				size_t posNvidia = std_codecName.find(L"NVIDIA");
				if (posNvidia != std::wstring::npos)
                {
                    codecList.insert(codecList.begin(), i);
                }
                else
                {
					codecList.push_back(i);
                }
            }
        }
	}

    void HevcEncoder::ResetAttempts()
    {
        numHEVCAttempts = 0;
	}

    void HevcEncoder::Register()
    {
        if (!mediaExtensionManager)
        {
            mediaExtensionManager = winrt::Windows::Media::MediaExtensionManager();
		}

		mediaExtensionManager.RegisterVideoEncoder(L"MagicBsod_Codecs_RT.HevcEncoder", MFVideoFormat_NV12, MFVideoFormat_HEVC);
	}

    void HevcEncoder::Register(IPropertySet configuration)
    {
        if (!mediaExtensionManager)
        {
            mediaExtensionManager = winrt::Windows::Media::MediaExtensionManager();
        }

        mediaExtensionManager.RegisterVideoEncoder(L"MagicBsod_Codecs_RT.HevcEncoder", MFVideoFormat_NV12, MFVideoFormat_HEVC, configuration);
	}

    UINT32 HevcEncoder::GetNumEncoders() {
		return numHevcEncoders;
    }

    winrt::hstring HevcEncoder::GetSelectedCodec()
    {
		return codecBeingUsed;
    }

    void HevcEncoder::SetProperties(IPropertySet)
    {
    }

    // ================= IMFTransform =================

    HRESULT __stdcall HevcEncoder::GetStreamLimits(
        DWORD* inputMin,
        DWORD* inputMax,
        DWORD* outputMin,
        DWORD* outputMax
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->GetStreamLimits(inputMin, inputMax, outputMin, outputMax);
    }

    HRESULT __stdcall HevcEncoder::GetStreamCount(
        DWORD* inputStreams,
        DWORD* outputStreams
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->GetStreamCount(inputStreams, outputStreams);
    }

    HRESULT __stdcall HevcEncoder::GetStreamIDs(
        DWORD inputSize,
        DWORD* inputIDs,
        DWORD outputSize,
        DWORD* outputIDs
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->GetStreamIDs(inputSize, inputIDs, outputSize, outputIDs);
    }

    HRESULT __stdcall HevcEncoder::GetInputStreamInfo(
        DWORD streamID,
        MFT_INPUT_STREAM_INFO* info
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->GetInputStreamInfo(streamID, info);
    }

    HRESULT __stdcall HevcEncoder::GetOutputStreamInfo(
        DWORD streamID,
        MFT_OUTPUT_STREAM_INFO* info
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->GetOutputStreamInfo(streamID, info);
    }

    HRESULT __stdcall HevcEncoder::GetAttributes(
        IMFAttributes** attributes
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->GetAttributes(attributes);
    }

    HRESULT __stdcall HevcEncoder::GetInputStreamAttributes(
        DWORD streamID,
        IMFAttributes** attributes
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->GetInputStreamAttributes(streamID, attributes);
    }

    HRESULT __stdcall HevcEncoder::GetOutputStreamAttributes(
        DWORD streamID,
        IMFAttributes** attributes
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->GetOutputStreamAttributes(streamID, attributes);
    }

    HRESULT __stdcall HevcEncoder::DeleteInputStream(
        DWORD streamID
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->DeleteInputStream(streamID);
    }

    HRESULT __stdcall HevcEncoder::AddInputStreams(
        DWORD count,
        DWORD* streamIDs
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->AddInputStreams(count, streamIDs);
    }

    HRESULT __stdcall HevcEncoder::GetInputAvailableType(
        DWORD streamID,
        DWORD typeIndex,
        IMFMediaType** mediaType
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->GetInputAvailableType(streamID, typeIndex, mediaType);
    }

    HRESULT __stdcall HevcEncoder::GetOutputAvailableType(
        DWORD streamID,
        DWORD typeIndex,
        IMFMediaType** mediaType
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->GetOutputAvailableType(streamID, typeIndex, mediaType);
    }

    HRESULT __stdcall HevcEncoder::SetInputType(
        DWORD streamID,
        IMFMediaType* type,
        DWORD flags
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->SetInputType(streamID, type, flags);
    }

    HRESULT __stdcall HevcEncoder::SetOutputType(
        DWORD streamID,
        IMFMediaType* type,
        DWORD flags
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        UINT32 quality;
		type->GetUINT32(MF_MT_MPEG2_LEVEL, &quality);
        auto codec{ m_wrappedEncoder.try_as<ICodecAPI>() };
        if (codec)
        {
            if (quality <= 100)
            {
                check_hresult(SetAttributeU32(codec, CODECAPI_AVEncCommonRateControlMode, eAVEncCommonRateControlMode_Quality));
				check_hresult(SetAttributeU32(codec, CODECAPI_AVEncCommonQuality, quality));
            }
            else
            {
                check_hresult(SetAttributeU32(codec, CODECAPI_AVEncCommonRateControlMode, eAVEncCommonRateControlMode_PeakConstrainedVBR));
                check_hresult(SetAttributeU32(codec, CODECAPI_AVEncCommonMaxBitRate, quality));
            }
        }

        return m_wrappedEncoder->SetOutputType(streamID, type, flags);
    }

    HRESULT __stdcall HevcEncoder::GetInputCurrentType(
        DWORD streamID,
        IMFMediaType** type
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->GetInputCurrentType(streamID, type);
    }

    HRESULT __stdcall HevcEncoder::GetOutputCurrentType(
        DWORD streamID,
        IMFMediaType** type
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->GetOutputCurrentType(streamID, type);
    }

    HRESULT __stdcall HevcEncoder::GetInputStatus(
        DWORD streamID,
        DWORD* flags
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->GetInputStatus(streamID, flags);
    }

    HRESULT __stdcall HevcEncoder::GetOutputStatus(
        DWORD* flags
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->GetOutputStatus(flags);
    }

    HRESULT __stdcall HevcEncoder::SetOutputBounds(
        LONGLONG lower,
        LONGLONG upper
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->SetOutputBounds(lower, upper);
    }

    HRESULT __stdcall HevcEncoder::ProcessEvent(
        DWORD streamID,
        IMFMediaEvent* event
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->ProcessEvent(streamID, event);
    }

    HRESULT __stdcall HevcEncoder::ProcessMessage(
        MFT_MESSAGE_TYPE message,
        ULONG_PTR param
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->ProcessMessage(message, param);
    }

    HRESULT __stdcall HevcEncoder::ProcessInput(
        DWORD streamID,
        IMFSample* sample,
        DWORD flags
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->ProcessInput(streamID, sample, flags);
    }

    HRESULT __stdcall HevcEncoder::ProcessOutput(
        DWORD flags,
        DWORD bufferCount,
        MFT_OUTPUT_DATA_BUFFER* buffers,
        DWORD* status
    ) noexcept
    {
        if (!m_wrappedEncoder) return E_FAIL;
        return m_wrappedEncoder->ProcessOutput(flags, bufferCount, buffers, status);
    }

    // ================= IMFMediaEventGenerator =================

    HRESULT __stdcall HevcEncoder::GetEvent(
        DWORD flags,
        IMFMediaEvent** event
    ) noexcept
    {
        if (!m_mediaEventGenerator) return E_FAIL;
        return m_mediaEventGenerator->GetEvent(flags, event);
    }

    HRESULT __stdcall HevcEncoder::BeginGetEvent(
        IMFAsyncCallback* callback,
        IUnknown* state
    ) noexcept
    {
        if (!m_mediaEventGenerator) return E_FAIL;
        return m_mediaEventGenerator->BeginGetEvent(callback, state);
    }

    HRESULT __stdcall HevcEncoder::EndGetEvent(
        IMFAsyncResult* result,
        IMFMediaEvent** event
    ) noexcept
    {
        if (!m_mediaEventGenerator) return E_FAIL;
        return m_mediaEventGenerator->EndGetEvent(result, event);
    }

    HRESULT __stdcall HevcEncoder::QueueEvent(
        MediaEventType type,
        REFGUID extendedType,
        HRESULT status,
        const PROPVARIANT* value
    ) noexcept
    {
        if (!m_mediaEventGenerator) return E_FAIL;
        return m_mediaEventGenerator->QueueEvent(type, extendedType, status, value);
    }

    // ================= IMFShutdown =================

    HRESULT __stdcall HevcEncoder::Shutdown() noexcept
    {
        if (!m_shutdown) return S_OK;
        return m_shutdown->Shutdown();
    }

    HRESULT __stdcall HevcEncoder::GetShutdownStatus(
        MFSHUTDOWN_STATUS* status
    ) noexcept
    {
        if (!m_shutdown) return E_FAIL;
        return m_shutdown->GetShutdownStatus(status);
    }
}