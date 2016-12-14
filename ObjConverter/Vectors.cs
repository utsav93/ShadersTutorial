
using System;
struct Vec2
{
    public Vec2(float x, float y)
    {
        this = new Vec2();
        X = x; Y = y;
    }
    public float X { get; set; }
    public float Y { get; set; }
}

struct Vec3
{
    public Vec3(float x, float y, float z)
    {
        this = new Vec3();
        X = x; Y = y; Z = z;
    }
    public float X { get; set; }
    public float Y { get; set; }
    public float Z { get; set; }
    public static Vec3 operator +(Vec3 left, Vec3 right)
    {
        return new Vec3 { X = left.X + right.X, Y = left.Y + right.Y, Z = left.Z + right.Z };
    }
    public static Vec3 operator -(Vec3 left, Vec3 right)
    {
        return new Vec3 { X = left.X - right.X, Y = left.Y - right.Y, Z = left.Z - right.Z };
    }
    public static Vec3 operator *(float f, Vec3 v)
    {
        return new Vec3(f * v.X, f * v.Y, f * v.Z);
    }
    public static Vec3 operator *(Vec3 v, float f)
    {
        return f * v;
    }
    public static Vec3 Normalize(Vec3 victim)
    {
        float f = (float)(1.0 / Math.Sqrt(Dot(victim, victim)));
        return f * victim;
    }
    public static float Dot(Vec3 left, Vec3 right)
    {
        return left.X * right.X + left.Y * right.Y + left.Z * right.Z;
    }
}

struct Vec4
{
    public Vec4(Vec3 vec3)
    {
        this = new Vec4();
        X = vec3.X;
        Y = vec3.Y;
        Z = vec3.Z;
        W = 0;
    }
    public float X { get; set; }
    public float Y { get; set; }
    public float Z { get; set; }
    public float W { get; set; }
}
