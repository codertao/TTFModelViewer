using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using TachyonExplorer.Deserializers;

namespace TachyonExplorer.CBIN
{
    public class CBINFile : BaseDeserializer
    {
        //CBINID is the first 4 bytes of the file and if it contains CBIN in them its a CBIN file.
        //TextDataOffset is a pointer to where in the file the null terminated string data is stored
        //TextDataSize tells you the total size of the string data.
        //TotalItems tells you the total numbe of items there are in all groups in the file.
        //EncryptionKey is the value used to encrypt this file and we use it to decrypt.
        public string Magic { get; set; }
        public int TextDataOffset { get; set; }
        public int TextDataLength { get; set; }
        public int TotalItems { get; set; }
        public uint EncryptionKey { get; set; }

        public List<CBINGroup> Groups { get; set; }

        public string ToINIString()
        {
            StringBuilder b = new StringBuilder();
            foreach (var group in Groups)
            {
                b.Append("[" + group.Value + "]\r\n");
                foreach (var item in group.Items)
                {
                    b.Append(item.Value + " = ");
                    b.Append(string.Join(", ", item.Parameters.Select(p => p.Value)));
                    b.Append("\r\n");
                }
                b.Append("\r\n");
            }
            return b.ToString();
        }


        public CBINFile(byte[] data) : base(data)
        {
            Magic = ReadBytes(0, 4).AsString();
            TextDataOffset = ReadInt32(4);
            TextDataLength = ReadInt32(8);
            TotalItems = ReadInt32(12);
            EncryptionKey = ReadUInt32(16);
            Groups = new List<CBINGroup>();

            if (Magic != "CBIN")
                throw new ApplicationException("Bad Magic");

            Deserialize();
        }

        private void Deserialize()
        {
            var bytes = Decrypt(ReadBytes(5 * 4, data.Length - 5 * 4), EncryptionKey);
            var reader = bytes.ToReader();

            int totalGroups = reader.ReadInt32();
            for (int i = 0; i < totalGroups; i++)
            {
                Groups.Add(new CBINGroup
                {
                    GroupId = reader.ReadInt32(),
                    TotalItems = reader.ReadInt32(),
                    Items = new List<CBINItem>()
                });
            }

            foreach (var group in Groups)
            {
                for (int j = 0; j < group.TotalItems; j++)
                {
                    group.Items.Add(new CBINItem
                    {
                        ItemId = reader.ReadInt32(),
                        TotalParameters = reader.ReadInt32(),
                        Parameters = new List<CBINParameter>()
                    });
                }
                if (group.TotalItems > 0) reader.ReadBytes(8);
            }

            foreach (var group in Groups)
                foreach (var item in group.Items)
                {
                    for (int i = 0; i < item.TotalParameters; i++)
                    {
                        item.Parameters.Add(new CBINParameter
                        {
                            Parameter = reader.ReadInt32(),
                            ParameterType = reader.ReadInt32()
                        });
                    }
                }

            List<string> strings = new List<string>();
            for (int i = 0; i < TotalItems; i++)
                strings.Add(reader.ReadNullTermString());

            foreach (var group in Groups)
            {
                group.Value = strings[group.GroupId - 1];
                foreach (var item in group.Items)
                {
                    item.Value = strings[item.ItemId - 1];
                    foreach (var param in item.Parameters)
                    {
                        switch (param.ParameterType)
                        {
                            case 1: //int, stored in parameter
                                param.Value = param.Parameter.ToString();
                                break;
                            case 2: //float
                                param.Value = param.Parameter.ConvertToFloat().ToString();
                                break;
                            case 4: //string
                                param.Value = strings[param.Parameter - 1];
                                break;
                            default:
                                throw new ApplicationException("Unknown param type " + param.ParameterType);
                        }
                    }
                }
            }
        }

        private uint RotateLeft(uint input, int bits)
        {
            return (input << bits) | (input >> (32 - bits));
        }

        private uint RotateRight(uint input, int bits)
        {
            return (input >> bits) | (input >> (32 - bits));
        }

        /*
        //AND OPERATION VALUES
        //Binary                           Hex
        //10000000000000000000000000000000 0x80000000 //clip all but signed bit
        //01111111100000000000000000000000 0x7F800000 //clip all but exponent bits 23 - 30
        //00000000011111111111111111111111 0x007FFFFF //clip all but significand bits 0 - 22
        //00000000000000000000000000000001 0x00000001 //clip everything except the first bit
         */

        private byte[] Decrypt(byte[] data, uint key)
        {
            byte[] ret = new byte[data.Length];
            for (int i = 0; i < data.Length; i++)
            {
                key = RotateLeft(key, 7);
                ret[i] = (byte)(data[i] ^ key);
            }
            return ret;
        }

    }
}
