using System;
using System.Collections.Generic;
using System.Linq;
using TachyonExplorer.Deserializers;

namespace TachyonExplorer.CBIN
{
    public class CBINGroup : BaseToString
    {
        //GroupID uses the TextData to tell you what text it uses.
        //TotalItems tell you the total number of items in this group.
        public int GroupId { get; set; }
        public int TotalItems { get; set; }

        public List<CBINItem> Items { get; set; }
        public string Value { get; set; }

        public string GetString(string name, string def)
        {
            return Items
                .Where(b => name.Equals(b.Value, StringComparison.InvariantCultureIgnoreCase))
                .SelectMany(i => i.Parameters.Select(p => p.Value))
                .DefaultIfEmpty(def)
                .First();
        }

        public int[] GetInts(string name)
        {
            Func<string, int?> getInt = s =>
            {
                int o;
                if (!Int32.TryParse(s, out o))
                    return null;
                return o;
            };
            return Items.Where(b => b.Value == name).SelectMany(i => i.Parameters.Select(p => getInt(p.Value))).Where(i => i.HasValue).Select(i => i.Value).ToArray();
        }

        public int GetInt(string name, int def)
        {
            return Int32.Parse(GetString(name, def + ""));
        }

        public double GetDouble(string name, double def)
        {
            return Double.Parse(GetString(name, def + ""));
        }

        public float GetFloat(string name, float def)
        {
            return Single.Parse(GetString(name, def + ""));
        }
    }
}
