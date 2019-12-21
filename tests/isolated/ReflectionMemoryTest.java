import java.lang.reflect.Field;
import java.lang.reflect.Method;

public class ReflectionMemoryTest {
    static boolean b;

    public static void main(String[] args) {
        ReflectionMemoryTest s = new ReflectionMemoryTest();
        Class cls = s.getClass();
        Field[] fields = cls.getDeclaredFields();
        Method[] methods = cls.getDeclaredMethods();
        System.out.println(fields.length);
        for (int i = 0; i < 100000; i++) {
            String a = (fields[0].getType()).toString();
            String b = (methods[0].getReturnType()).toString();
            System.out.print(i);
        }
    }
}
