using System;
using System.IO;
using System.Collections.Generic;
using System.Diagnostics;

// Written in one hour while riding the train
class MyClass
{
    // Raw data from OBJ file
    static List<Vec3> positions = new List<Vec3>();
    static List<Vec2> uvCoords = new List<Vec2>();
    static List<Vec3> normals = new List<Vec3>();

    // Verts as we need them
    static List<VertexCombo> vertexCombos = new List<VertexCombo>();
    static List<Vertex> vertices = new List<Vertex>();
    // Indices into vertexCombos
    static List<ushort> indices = new List<ushort>();

    struct Vertex
    {
        public Vec3 position;
        public Vec3 normal;
        public Vec2 uv;
        public Vec4 tangent;
    }

    static int Main(string[] args)
    {
        if (args.Length != 2)
            return 0;
        try
        {
            Console.WriteLine("Reading obj file...");
            ReadObjData(args[0]);
            // RotateScene(); // For Maya files
            Console.WriteLine("Writing file...");
            WriteFile(args[1]);
        }
        catch (Exception e)
        {
            Console.WriteLine(e);
            return 0;
        }
        return 0;
    }

    static void RotateScene()
    {
        // Have to rotate 90 degrees from Maya
        // Swap components and negate one of them (sound familiar? C++ exericise)
        Console.WriteLine("Rotating...");
        for (int i = 0; i < positions.Count; i++)
        {
            // cos -sin
            // sin  cos
            Vec3 p = positions[i];
            float y = p.Y;
            p.Y = -p.Z;
            p.Z = y;
            positions[i] = p;
        }
    }

    static void ReadObjData(string objFileName)
    {
        StreamReader sr = new StreamReader(objFileName);
        string line;
        while ((line = sr.ReadLine()) != null)
        {
            string[] lineParts = line.Replace("  ", " ").Replace("//", "/").Split();
            switch (lineParts[0])
            {
                case "#":
                    break; // Comment
                case "v":
                    positions.Add(Parse3(lineParts));
                    break;
                case "vt":
                    if (lineParts.Length == 4)
                    {
                        // Texture has a w component
                        Vec3 t = Parse3(lineParts);
                        if (t.Z != 0)
                        {
                            t.X /= t.Z;
                            t.Y /= t.Z;
                        }
                        uvCoords.Add(new Vec2(t.X, t.Y));
                    }
                    else
                        uvCoords.Add(Parse2(lineParts));
                    break;
                case "vn":
                    normals.Add(Parse3(lineParts));
                    break;
                case "usemtl":
                    break;
                case "f":
                    AddFace(lineParts);
                    break;
                default:
                    //Console.WriteLine("Unreconized token: " + lineParts[0]);
                    break;
            }
        }
        sr.Close();
    }

    static void WriteFile(string binaryFileName)
    {
        Random randy = new Random();
        // numVerts (int)
        // numIndices (int)
        // Verts (vec3 position, vec4 color, vec3 normal, vec2 uv)
        // indices (ushort)

        if (File.Exists(binaryFileName))
            File.Delete(binaryFileName);

        string path = Path.Combine(Environment.CurrentDirectory, binaryFileName);

        BinaryWriter bw = new BinaryWriter(File.Open(binaryFileName, FileMode.CreateNew));
        bw.Write(vertexCombos.Count);
        bw.Write(indices.Count);
        foreach (VertexCombo combo in vertexCombos)
        {
            Vertex vert;
            vert.position = positions[combo.aVertIndex];
            vert.uv = uvCoords[combo.bTextureIndex];
            vert.normal = normals[combo.cNormalIndex];
            vert.tangent = new Vec4(); // Calculated later
            vertices.Add(vert);
        }
        // uncomment  for Ogre model
        for (int i = 0; i < vertices.Count; i++)
        {
            Vertex t = vertices[i];
            t.uv.Y = 1 - t.uv.Y;
            vertices[i] = t;
        }

        Console.WriteLine("Computing tangents...");
        ComputeTangentVectors();
        Console.WriteLine("Now writing...");
        int negCount = 0;
        int posCount = 0;
        foreach (Vertex v in vertices)
        {
            // Position
            bw.Write(v.position.X);
            bw.Write(v.position.Y);
            bw.Write(v.position.Z);
            // Normal
            bw.Write(v.normal.X);
            bw.Write(v.normal.Y);
            bw.Write(v.normal.Z);
            // UV
            bw.Write(v.uv.X);
            bw.Write(v.uv.Y);
            // Tangent
            bw.Write(v.tangent.X);
            bw.Write(v.tangent.Y);
            bw.Write(v.tangent.Z);
            bw.Write(v.tangent.W);
            if (v.tangent.W < 0) negCount++; else posCount++;
        }

        foreach (ushort i in indices)
            bw.Write(i);
        bw.Close();
    }

    private static void ComputeTangentVectors()
    {
        Vec3[] tan1Accum = new Vec3[vertices.Count];
        Vec3[] tan2Accum = new Vec3[vertices.Count];
        Vec4[] tangents = new Vec4[vertices.Count];

        int facesSize = indices.Count / 3;
        List<ushort> faces = indices;

        // Compute the tangent vector
        for (int i = 0; i < facesSize; i += 3)
        {

            Vec3 p1 = vertices[(int)faces[i]].position;
            Vec3 p2 = vertices[(int)faces[i + 1]].position;
            Vec3 p3 = vertices[(int)faces[i + 2]].position;

            Vec2 tc1 = vertices[(int)faces[i]].uv;
            Vec2 tc2 = vertices[(int)faces[i + 1]].uv;
            Vec2 tc3 = vertices[(int)faces[i + 2]].uv;

            Vec3 q1 = p2 - p1;
            Vec3 q2 = p3 - p1;
            float s1 = tc2.X - tc1.X, s2 = tc3.X - tc1.X;
            float t1 = tc2.Y - tc1.Y, t2 = tc3.Y - tc1.Y;
            float r = 1.0f / (s1 * t2 - s2 * t1);
            Vec3 tan1 = new Vec3(
                (t2 * q1.X - t1 * q2.X) * r,
                (t2 * q1.Y - t1 * q2.Y) * r,
                (t2 * q1.Z - t1 * q2.Z) * r);
            Vec3 tan2 = new Vec3(
                (s1 * q2.X - s2 * q1.X) * r,
                (s1 * q2.Y - s2 * q1.Y) * r,
                (s1 * q2.Z - s2 * q1.Z) * r);
            tan1Accum[(int)faces[i]] += tan1;
            tan1Accum[(int)faces[i + 1]] += tan1;
            tan1Accum[(int)faces[i + 2]] += tan1;
            tan2Accum[(int)faces[i]] += tan2;
            tan2Accum[(int)faces[i + 1]] += tan2;
            tan2Accum[(int)faces[i + 2]] += tan2;
        }

        int dang = 0;
        for (int i = 0; i < vertices.Count; ++i)
        {
            Vec3 n = normals[i];
            Vec3 t1 = tan1Accum[i];
            Vec3 t2 = tan2Accum[i];

            // Gram-Schmidt orthogonalize
            tangents[i] = new Vec4(normalize(t1 - (dot(n, t1) * n)));
            // Store handedness in w
            tangents[i].W = (dot(cross(n, t1), t2) < 0.0f) ? -1.0f : 1.0f;
            tangents[i].W = -1.0f;
            Vertex t = vertices[i];
            t.tangent = tangents[i];
            vertices[i] = t;
            float epsilon = 0.3f;
            if (length(t.tangent) < epsilon || float.IsNaN(length(t.tangent))
                )
            {
                dang++;
                Console.WriteLine("asdf");
            }
        }
        Console.WriteLine(dang);
    }

    private static float length(Vec4 v)
    {
        return (float)Math.Sqrt(dot(v, v));
    }

    private static float dot(Vec3 v1, Vec3 v2)
    {
        return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z;
    }
    private static float dot(Vec4 v1, Vec4 v2)
    {
        return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z;
    }

    private static Vec3 cross(Vec3 v1, Vec3 v2)
    {
        return new Vec3 { 
            X = v1.Y * v2.Z - v1.Z * v2.Y,
            Y = v1.Z * v2.X - v1.X * v2.Z,
            Z = v1.X * v2.Y - v1.Y * v2.X 
        };
    }

    private static Vec3 normalize(Vec3 v)
    {
        float magnitude = (float)Math.Sqrt(dot(v, v));
        Vec3 ret = v;
        ret.X /= magnitude;
        ret.Y /= magnitude;
        ret.Z /= magnitude;
        return ret;
    }


    /// <summary>
    /// Adds a line of triangle data to our in-memory data.
    /// (ex: "1/1/1 2/4/9 8/4/2").
    /// </summary>
    /// <param name="lineParts"></param>
    static void AddFace(string[] lineParts)
    {
        VertexCombo one = MakeVertexCombo(lineParts[1]);
        VertexCombo two = MakeVertexCombo(lineParts[2]);
        VertexCombo three = MakeVertexCombo(lineParts[3]);
        addIndex(one);
        addIndex(two);
        addIndex(three);
    }

    /// <summary>
    /// Detects if this VertexCombo has been seen before. If so, we just
    /// use the index of the exising vertex. Otherwise we add this combo
    /// and use its index instead.
    /// </summary>
    /// <param name="combo"></param>
    static void addIndex(VertexCombo combo)
    {
        ushort index = (ushort)vertexCombos.IndexOf(combo);
        if (index == unchecked((ushort)-1))
        {
            vertexCombos.Add(combo);
            index = (ushort)(vertexCombos.Count - 1);
        }
        indices.Add(index);
    }

    /// <summary>
    /// Converts the string of index (ex: "1/4/3") data into a VertexCombo.
    /// </summary>
    static VertexCombo MakeVertexCombo(string vertIndices)
    {
        VertexCombo ret = new VertexCombo();
        string[] asParts = vertIndices.Split('/');
        ret.aVertIndex = int.Parse(asParts[0]) - 1;
        ret.bTextureIndex = int.Parse(asParts[1]) - 1;
        ret.cNormalIndex = int.Parse(asParts[2]) - 1;
        return ret;
    }

    /// <summary>
    /// Converts lineParts[1] and [2] into a Vec2.
    /// </summary>
    static Vec2 Parse2(string[] lineParts)
    {
        Debug.Assert(lineParts.Length == 3);
        return new Vec2 { X = float.Parse(lineParts[1]), Y = float.Parse(lineParts[2]) };
    }

    /// <summary>
    /// Converts lineParts[1] and [2] and [3] into a Vec3.
    /// </summary>
    static Vec3 Parse3(string[] lineParts)
    {
        Debug.Assert(lineParts.Length == 4);
        return new Vec3 { X = float.Parse(lineParts[1]), Y = float.Parse(lineParts[2]), Z = float.Parse(lineParts[3]) };
    }
}
