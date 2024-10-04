import dna.DNA;

import java.util.ArrayList;
public class  MAIN {

    public static void main(String[] args)
    {
        String x  = "ATGCCAACATGGATGCCCGATAT++GGATTG+A!";
        String y = x.substring(0,3);
        System.out.println(y);

        if(y.equals("ATG"))
            System.out.println("BARK");

        else
            System.out.println("MEOW");

    }
}
