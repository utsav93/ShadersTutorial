
/// <summary>
/// Represents the indices into the three elements that makes
/// a single vert of an obj face.
/// </summary>
class VertexCombo
{
    public int aVertIndex { get; set; }
    public int bTextureIndex { get; set; }
    public int cNormalIndex { get; set; }
    public override bool Equals(object obj)
    {
        VertexCombo other = obj as VertexCombo;
        return other.aVertIndex == aVertIndex && other.bTextureIndex == bTextureIndex && other.cNormalIndex == cNormalIndex;
    }
}