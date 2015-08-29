using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Xml.Serialization;

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
                using (var stream = File.OpenWrite(SettingsFile))
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
    }

    [Serializable]
    public class AppSetting
    {
        public string Key { get; set; }
        public string Value { get; set; }
    }
}
