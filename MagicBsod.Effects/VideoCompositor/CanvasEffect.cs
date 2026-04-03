using System;
using Microsoft.Graphics.Canvas;
using Windows.Foundation.Collections;
using Windows.Graphics.DirectX.Direct3D11;
using Windows.Media.Effects;
using Windows.Media.MediaProperties;
using MagicBsod_Effects_RT.Interfaces;
using MagicBsod_Effects_RT.VideoCompositor;

namespace MagicBsod.Effects.VideoCompositor;

public class CanvasEffect : IStudioVideoCompositorEffect
{
    private VideoEncodingProperties? _backgroundProperties;
    private CanvasDevice? _canvasDevice;
    private IPropertySet? _configuration;

    static CanvasEffect()
    {
        StudioVideoCompositorEffectTemplate.ActivatableClassConstructor = () => new CanvasEffect();
    }

    public static VideoCompositorDefinition CreateCompositorEffect(IPropertySet configuration)
    {
        return new VideoCompositorDefinition(typeof(StudioVideoCompositorEffectTemplate).FullName, configuration);
    }

    public bool TimeIndependent => throw new NotImplementedException();

    public void Close(MediaEffectClosedReason reason)
    {
    }

    public void CompositeFrame(CompositeVideoFrameContext context)
    {
    }

    public void DiscardQueuedFrames()
    {
    }

    public void SetEncodingProperties(VideoEncodingProperties encodingProperties, IDirect3DDevice device)
    {
    }

    public void SetProperties(IPropertySet configuration)
    {
    }
}
