using System;
using System.Collections.Generic;
using System.Linq;
using TachyonExplorer.CBIN;
using TachyonExplorer.Deserializers;

namespace TachyonExplorer.OCF
{
    public class OCFFile : BaseToString
    {
        private CBINFile cbin;

        public string Name { get; set; }
        public List<OCFPart> Parts { get; set; } 
        public List<OCFPak> Paks { get; set; } 

        public OCFFile(byte[] data)
        {
            cbin = new CBINFile(data);
            LoadHeader();
            LoadParts();
            LoadPaks();
        }

        private void LoadHeader()
        {
            Name = cbin.Groups.Single(p => p.Value == "header").GetString("NAME", "NO_NAME");
        }

        private void LoadParts()
        {
            Parts = new List<OCFPart>();
            foreach (var group in cbin.Groups.Where(g => g.Value.StartsWith("part")))
            {
                var p = new OCFPart();
                p.Name = group.GetString("NAME", "");
                p.PAKName = group.GetString("PAK", "");
                p.Scale = group.GetDouble("SCALE", 1);
                p.AttachTo = group.GetString("ATTACH", null);
                if (p.AttachTo != null)
                    p.AttachToParams = group.GetInts("ATTACH");
                p.Angles = group.GetInts("ANGLES");
                p.Type = group.GetInt("TYPE", 0);
                p.SubType = group.GetInt("SUB_TYPE", 0);
                p.LandDock = group.GetInts("LAUNCH_DOCK");
                p.PartId = group.GetInt("PART_ID", 0);
                Parts.Add(p);
            }
        }

        private void LoadPaks()
        {
            foreach (var item in cbin.Groups.Single(c => c.Value.EqualsCI("paks")).Items.Where(i => i.Value.EqualsCI("PAK")))
            {
                Paks.Add(new OCFPak
                {
                    Name = item.Parameters[0].Value,
                    PAKFileName = item.Parameters[1].Value,
                    PALFileName = item.Parameters[2].Value,
                    Unknown = item.Parameters[3].IntValue,
                    Type = item.Parameters[4].IntValue
                });
            }
        }
    }
}
