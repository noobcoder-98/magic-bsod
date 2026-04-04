using MagicBsod.Effects.VideoCompositor;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Controls.Primitives;
using Microsoft.UI.Xaml.Data;
using Microsoft.UI.Xaml.Input;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Navigation;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Media;
using Windows.Media.Editing;
using Windows.Media.Effects;
using Windows.Media.MediaProperties;
using Windows.Media.Transcoding;
using Windows.Storage;
using Windows.Storage.Streams;
using Windows.UI;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace MagicBsod.UI
{
    /// <summary>
    /// An empty window that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainWindow : Window
    {
        private readonly Dictionary<MediaOverlay, (bool, Color, double)> _overlayBorders = new Dictionary<MediaOverlay, (bool, Color, double)>();
        private MediaEncodingProfile? _profileHD;
        private MediaEncodingProfile? _profile4K;
        public MainWindow()
        {
            InitializeComponent();
            InitProfile();
        }

        private void InitProfile()
        {
            MediaExtensionManager manager = new MediaExtensionManager();
            //// HD profile (1080p)
            //_profileHD = MediaEncodingProfile.CreateMp4(VideoEncodingQuality.HD1080p);
            //_profileHD.Video.Width = 1920;
            //_profileHD.Video.Height = 1080;
            //_profileHD.Video.FrameRate.Numerator = 30;
            //_profileHD.Video.FrameRate.Denominator = 1;
            //_profileHD.Video.Bitrate = 75;

            // 4k profile
            _profile4K = MediaEncodingProfile.CreateHevc(VideoEncodingQuality.Uhd2160p);
            _profile4K.Video.Width = 3840;
            _profile4K.Video.Height = 3840;
            _profile4K.Video.FrameRate.Numerator = 60;
            _profile4K.Video.FrameRate.Denominator = 1;
            _profile4K.Video.Bitrate = 20_000_000; 
            var key = new Guid("96f66574-11c5-4015-8666-bff516436da7");
            _profile4K.Video.Properties[key] = 186;
        }

        private async void OnExportClick(object sender, RoutedEventArgs e)
        {
            for (int i = 0; i < 100; i++)
            {
                await ExportVideo(i);
            }
        }

        private async Task ExportVideo(int countVideo)
        {
            string outputFolderPath = @"C:\Users\supernoobcoder\Downloads\Magic";
            StorageFolder folder = await StorageFolder.GetFolderFromPathAsync(outputFolderPath);
            StorageFile outputFile = await folder.CreateFileAsync($"MagicExportVideo_{countVideo}.mp4", CreationCollisionOption.ReplaceExisting);

            var composition = await GenerateCompositionAsync();
            var transcoder = new MediaTranscoder();
            using IRandomAccessStream outStream = await outputFile.OpenAsync(FileAccessMode.ReadWrite);
            var prepare = await transcoder.PrepareMediaStreamSourceTranscodeAsync(
                composition.GenerateMediaStreamSource(_profile4K),
                outStream,
                _profile4K);

            try
            {
                if (prepare.CanTranscode)
                {
                    await prepare.TranscodeAsync();
                    ExportMagic.Content = $"Success export: Count {countVideo}";
                }
            }
            catch (Exception ex)
            {
                ExportMagic.Content = $"Exception: {ex.Message}";
            }
        }

        public async Task<MediaComposition> GenerateCompositionAsync()
        {
            PropertySet compositorConfigs = new PropertySet()
            {
                { "OverlayBorders", _overlayBorders }
            };

            VideoCompositorDefinition compositor = CanvasEffect.CreateCompositorEffect(compositorConfigs);

            MediaComposition composition = new MediaComposition();

            MediaClip clip = await GetMediaClipAsync();
            MediaOverlayLayer layer = new MediaOverlayLayer(compositor);
            MediaOverlay mediaOverlay = await GetMediaOverlayAsync();
            layer.Overlays.Add(mediaOverlay);
            composition.OverlayLayers.Add(layer);
            composition.Clips.Add(clip);
            return composition;
        }

        private async Task<MediaOverlay> GetMediaOverlayAsync()
        {
            string overlayPath = @"C:\Users\supernoobcoder\Pictures\Camera Roll\WIN_20260310_18_15_08_Pro.jpg";
            StorageFile overlayFile = await StorageFile.GetFileFromPathAsync(overlayPath);
            MediaClip mediaClip = await MediaClip.CreateFromImageFileAsync(overlayFile, TimeSpan.FromSeconds(5));
            MediaOverlay mediaOverlay = new(mediaClip)
            {
                AudioEnabled = false,
                Delay = TimeSpan.FromSeconds(1),
                Opacity = 0.5,
                Position = new Rect
                {
                    X = 50,
                    Y = 50,
                    Width = 500,
                    Height = 500
                }
            };
            return mediaOverlay;
        }

        private async Task<MediaClip> GetMediaClipAsync()
        {
            string inputPath = @"C:\Users\supernoobcoder\Downloads\screenshot_2.jpeg";
            StorageFile inputFile = await StorageFile.GetFileFromPathAsync(inputPath);
            MediaClip clip = await MediaClip.CreateFromImageFileAsync(inputFile, TimeSpan.FromSeconds(5));
            return clip;
        }
    }
}
