import java.util.HashSet;
package dna;

public class DNA {

    private String given;
    private String afterjunk;

    private double sum = 0;

    private double sumA = 0;
    private double sumT = 0;

    private double sumG = 0;

    private double sumC = 0;

    private double sumJ = 0;

    private int count = 0;
    public DNA (String given)
    {
        this.given = given;
        String junkafter ="";

        for(int i=0; i<given.length(); i++)
        {
            if(given.charAt(i)!='A' || given.charAt(i)!='T' || given.charAt(i)!='G' || given.charAt(i)!='C')
            {
                junkafter = given.replace(given.substring(i,i),"");
            }
        }

        this.afterjunk = junkafter;

        if(!valid(given))
        {
            throw new IllegalArgumentException("Invalid DNA sequence");
        }
    }



    boolean isProtein()
    {
        boolean result = false;

        totalMass();

        if(afterjunk.length()%3!=0)
            return false;

        else
        {
            if(afterjunk.length()>=15)
            {
                if((sumC+sumG)/sumT>=0.3)
                {
                    result = true;
                }

                else
                    return result;
            }

            else
                return result;
        }

        return result;
    }

    double totalMass() {


        for (int i = 0; i < given.length(); i++) {
            if (given.charAt(i) == 'A') {
                sumA += 135.128;
            } else if (given.charAt(i) == 'T') {
                sumT += 125.107;
            } else if (given.charAt(i) == 'G') {
                sumG += 151.128;
            } else if (given.charAt(i) == 'C') {
                sumC += 111.103;
            } else {
                sumJ += 100.000;
            }


        }
        sum = sumA + sumT + sumG + sumC + sumJ;

        sum = Math.round(sum*10.0)/10.0;

        return sum;
    }

    int nucleotideCount(char c)
    {

        for(int i=0; i<given.length(); i++)
        {
            if(given.charAt(i)==c)
            {
                count++;
            }
        }

        return count;
    }

    HashSet codonSet()
    {
        HashSet<String> set = new HashSet<String>();

        for(int i=0; i<afterjunk.length()-3;i++)
        {
            set.add(afterjunk.substring(i,i+3));
        }

        return set;
    }

    String mutateCodon(String originalCodon, String newCodon)
    {
        String newsequence = "";
        for(int i=0; i<originalCodon.length(); i++)
        {
            if(given.charAt(i)!='A' || given.charAt(i)!='T' || given.charAt(i)!='G' ||
                    given.charAt(i)!='C' ) {
                newsequence = given.replace(given.substring(i,i),"");
            }
        }

        for(int i=0; i<given.length()-3; i++)
        {
            if(newsequence.substring(i,i+3).equals(originalCodon))
            {
                newsequence.substring(i,i+3).replaceAll(newsequence.substring(i,i+3),newCodon);
            }
        }


        return newsequence;

    }

    String sequence()
    {
        return given;
    }

    boolean valid(String afterjunk)
    {
        boolean result = true;

        if(afterjunk.length()%3!=0)
            return false;

        return result;
    }

}
