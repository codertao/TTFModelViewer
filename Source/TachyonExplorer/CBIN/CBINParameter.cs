using System;
using TachyonExplorer.Deserializers;

namespace TachyonExplorer.CBIN
{
    public class CBINParameter : BaseToString
    {
        //Parameter can be int,float,string.
        //ParameterType lets you know the type of data Parameter is.
        //If Parameter is string type it uses the TextData to look up what text it uses.
        public int Parameter { get; set; }
        public int ParameterType { get; set; }

        public string Value { get; set; }
        public int IntValue { get { return Int32.Parse(Value); } }
    }
}
