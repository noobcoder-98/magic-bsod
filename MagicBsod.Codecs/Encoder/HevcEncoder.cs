using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MagicBsod.Codecs.Encoder
{
    public sealed class HevcEncoder
    {
        public static void Register()
        {
            MagicBsod.Codecs.RT.HevcEncoder.Register();
        }
    }
}
