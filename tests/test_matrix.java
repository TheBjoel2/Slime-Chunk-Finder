public class test_matrix
{
    private static long getSeed(long seed, int x, int z)
    {
        return (
                seed +
                (int) (x * x * 0x4c1906) +
                (int) (x * 0x5ac0db) +
                (int) (z * z) * 0x4307a7L +
                (int) (z * 0x5f24f) ^ 0x3ad8025fL
        );
    }

    public static void main(String[] args)
    {
        System.out.println(Long.toUnsignedString(getSeed(123456789, 82642+7, -21721+110)));
        System.out.println(Long.toUnsignedString(getSeed(123456789, 82642+200, -21721+0)));
        System.out.println(Long.toUnsignedString(getSeed(123456789, 82642+32, -21721+52)));
    }
}
