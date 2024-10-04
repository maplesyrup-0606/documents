package dna;
import java.util.HashSet;
import java.util.ArrayList;


public class DNA {

    private String given;
    private ArrayList<String> list = new ArrayList<>();
    private String afterjunk = "";

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
        this.afterjunk = removejunk(given);

        if(!valid(given))
        {
            throw new IllegalArgumentException("Invalid DNA sequence");
        }
    }



    boolean isProtein()
    {
        boolean result = false;
        String link = "";
        link += afterjunk;
        totalMass();
        int start = link.length()-3;
        int end = link.length();

        if(!(link.substring(0,3).equals("ATG") && (link.substring(start,end).equals("TAA") || link.substring(start,end).equals("TAG") || link.substring(start,end).equals("TGA"))))
            return false;
        if(link.length()%3!=0)
            return false;

        else
        {
            if(link.length()>=15)
                {
                    if((sumC+sumG)/sumT>=0.3)
                    {
                        result = true;
                    }

                    else
                        return false;
                }

                else
                    return false;
        }


        return result;
    }





    double totalMass() {


        for (int i = 0; i < given.length(); i++) {
            if (given.charAt(i) == 'A') {
                this.sumA += 135.128;
            } else if (given.charAt(i) == 'T') {
                this.sumT += 125.107;
            } else if (given.charAt(i) == 'G') {
                this.sumG += 151.128;
            } else if (given.charAt(i) == 'C') {
                this.sumC += 111.103;
            } else {
                this.sumJ += 100.000;
            }


        }
        this.sum = sumA + sumT + sumG + sumC + sumJ;

        this.sum = Math.round(sum*10.0)/10.0;

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
        HashSet<String> set = new HashSet<>();
        String link2 = "";
        link2 += afterjunk;
        for(int i=0; i<link2.length(); i+=3)
        {
            set.add(link2.substring(i,i+3));
        }

        return set;
    }

    String mutateCodon(String originalCodon, String newCodon)
    {
        if(removejunk(newCodon).length()%3==0)
        {
            this.given = removejunk(given);

            given = given.replaceAll(originalCodon,newCodon);
        }


        return given;
    }

    String sequence()
    {
        return given;
    }

    boolean valid(String given)
    {
        boolean result = true;

        if(afterjunk.length()%3!=0)
            return false;

        return result;
    }

    String removejunk (String beforejunk)
    {
        String removedjunk = "";
        ArrayList<String> list = new ArrayList<>();

        for(int i=0; i<beforejunk.length(); i++)
        {
            list.add(String.valueOf(beforejunk.charAt(i)));

        }


        for(int i=0; i<list.size(); i++)
        {
            if( list.get(i).equals("A") || list.get(i).equals("T") || list.get(i).equals("G") || list.get(i).equals("C"))
                removedjunk += list.get(i);
        }

        return removedjunk;
    }

}
