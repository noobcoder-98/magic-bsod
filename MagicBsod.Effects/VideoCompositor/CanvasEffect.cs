using System;
using Microsoft.Graphics.Canvas;
using Windows.Foundation.Collections;
using Windows.Graphics.DirectX.Direct3D11;
using Windows.Media.Effects;
using Windows.Media.MediaProperties;
using MagicBsod_Effects_RT.Interfaces;
using MagicBsod_Effects_RT.VideoCompositor;
using Microsoft.UI;
using Windows.Media.Editing;
using Windows.Foundation;

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
        using CanvasBitmap backgroundBitmap = CanvasBitmap.CreateFromDirect3D11Surface(_canvasDevice, context.BackgroundFrame.Direct3DSurface);
        using CanvasRenderTarget renderTarget = CanvasRenderTarget.CreateFromDirect3D11Surface(_canvasDevice, context.OutputFrame.Direct3DSurface);
        using CanvasDrawingSession ds = renderTarget.CreateDrawingSession();
        ds.Clear(Colors.Transparent);
        ds.Antialiasing = CanvasAntialiasing.Antialiased;
        ds.DrawImage(backgroundBitmap);
        foreach (IDirect3DSurface? surface in context.SurfacesToOverlay)
        {
            using CanvasBitmap inputBitmap = CanvasBitmap.CreateFromDirect3D11Surface(_canvasDevice, surface);
            MediaOverlay overlay = context.GetOverlayForSurface(surface);
            Rect sourceRectangle = GetSourceRectangle(inputBitmap, overlay);
            ds.DrawImage(inputBitmap, sourceRectangle);
        }
    }

    private Rect GetSourceRectangle(CanvasBitmap inputBitmap, MediaOverlay overlay)
    {
        Point diffSize = new();
        Point diffOffset = new();
        SetDiffHeight(overlay, ref diffSize, ref diffOffset);
        SetDiffWidth(overlay, ref diffSize, ref diffOffset);
        Rect sourceRectangle = new(inputBitmap.Bounds.X + diffOffset.X, inputBitmap.Bounds.Y + diffOffset.Y, inputBitmap.Bounds.Width + diffSize.X, inputBitmap.Bounds.Height + diffSize.Y);
        return sourceRectangle;
    }

    private void SetDiffWidth(MediaOverlay overlay, ref Point diffSize, ref Point diffOffset)
    {
        if (!overlay.Clip.UserData.TryGetValue("DiffWidth", out string val))
        {
            return;
        }

        _ = double.TryParse(val, out double diffwidth);
        _ = double.TryParse(overlay.Clip.UserData["OffsetX"], out double offsetX);

        diffSize.X = diffwidth;
        diffOffset.X = offsetX;
    }

    private void SetDiffHeight(MediaOverlay overlay, ref Point diffSize, ref Point diffOffset)
    {
        if (!overlay.Clip.UserData.TryGetValue("DiffHeight", out string val))
        {
            return;
        }

        _ = double.TryParse(val, out double diffHeight);
        _ = double.TryParse(overlay.Clip.UserData["OffsetY"], out double offsetY);
        diffSize.Y = diffHeight;
        diffOffset.Y = offsetY;
    }

    public void DiscardQueuedFrames()
    {
    }

    public void SetEncodingProperties(VideoEncodingProperties encodingProperties, IDirect3DDevice device)
    {
        _backgroundProperties = encodingProperties;
        _canvasDevice = CanvasDevice.CreateFromDirect3D11Device(device);
        Size compositionSize = new Size((int)_backgroundProperties.Width, (int)_backgroundProperties.Height);


    }

    public void SetProperties(IPropertySet configuration)
    {
    }
}
