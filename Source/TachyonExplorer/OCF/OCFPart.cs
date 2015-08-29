namespace TachyonExplorer.OCF
{
    public class OCFPart
    {
        public string Name { get; set; }
        public string PAKName { get; set; }
        public double Scale { get; set; }
        public int[] Angles { get; set; }
        public int Type { get; set; }
        public int SubType { get; set; }
        public string AttachTo { get; set; }
        public int[] AttachToParams { get; set; }
        public int PartId { get; set; }
        public int[] LandDock { get; set; }
    }
}
