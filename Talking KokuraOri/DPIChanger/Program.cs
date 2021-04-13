using System.Drawing;

namespace DpiChanger
{
    class Program
    {
        static readonly string format = ".png";
        static readonly string outputPaht = "../../../../output/";

        static void Main(string[] args)
        {
            if(args.Length <= 0)
            {
                return;
            }

            var bmpOrg = new Bitmap(outputPaht + args[0] + format);
            var bmpNew = new Bitmap(bmpOrg.Width, bmpOrg.Height);
            bmpNew.SetResolution(300f, 300f);
            var graphics = Graphics.FromImage(bmpNew);
            graphics.DrawImage(bmpOrg, 0, 0, bmpOrg.Width, bmpOrg.Height);
            graphics.Dispose();
            bmpOrg.Dispose();
            bmpNew.Save(outputPaht + args[0] + format);
            bmpNew.Dispose();

            return;
        }
    }
}
