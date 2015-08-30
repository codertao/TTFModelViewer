using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Xml.Serialization;
using Nexus;

namespace TachyonExplorer
{
    public static class AppSettings
    {
        private const string SettingsFile = "settings.xml";
        private static Dictionary<string, string> settings = new Dictionary<string, string>(); 
        static AppSettings()
        {
            Load();
        }

        public static void Load()
        {
            try
            {
                if (File.Exists(SettingsFile))
                {
                    using (var stream = File.OpenRead(SettingsFile))
                    {
                        var ser = new XmlSerializer(typeof(List<AppSetting>));
                        var data = ser.Deserialize(stream) as List<AppSetting>;
                        foreach (var setting in data)
                            settings[setting.Key] = setting.Value;
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }
        }

        public static void Save()
        {
            try
            {
                using (var stream = File.Open(SettingsFile, FileMode.Create))
                {
                    var data = settings.Select(kvp => new AppSetting{Key=kvp.Key, Value=kvp.Value}).ToList();
                    var ser = new XmlSerializer(typeof(List<AppSetting>));
                    ser.Serialize(stream, data);
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }
        }


        public static String GetString(string key, string def = null)
        {
            return settings.ContainsKey(key) ? settings[key] : def;
        }

        public static void SetString(string key, string value)
        {
            settings[key] = value;
        }

        public static float[] GetFloats(string key, float[] defs = null)
        {
            if (settings.ContainsKey(key))
            {
                string[] vals = settings[key].Split('\t');
                float[] fVals = new float[vals.Length];
                for (int i = 0; i < vals.Length; i++)
                    if (!Single.TryParse(vals[i], out fVals[i]))
                        return defs;
                return fVals;
            }
            return defs;
        }

        public static void SetFloats(string key, float[] fVals)
        {
            SetString(key, fVals == null ? "null" : String.Join("\t", fVals.Select(f=>f.ToString())));
        }



        public static Point3D GetPoint3D(string key, Point3D def = default(Point3D))
        {
            var vals = GetFloats(key);
            return vals == null || vals.Length != 3 ? def : new Point3D(vals[0], vals[1], vals[2]);
        }

        public static void SetPoint3D(string key, Point3D value)
        {
            SetFloats(key, new []{value.X, value.Y, value.Z});
        }



        public static Vector3D GetVector3D(string key, Vector3D def = default(Vector3D))
        {
            var vals = GetFloats(key);
            return vals == null || vals.Length != 3 ? def : new Vector3D(vals[0], vals[1], vals[2]);
        }

        public static void SetVector3D(string key, Vector3D value)
        {
            SetFloats(key, new[] { value.X, value.Y, value.Z });
        }
    }

    [Serializable]
    public class AppSetting
    {
        public string Key { get; set; }
        public string Value { get; set; }
    }
}
