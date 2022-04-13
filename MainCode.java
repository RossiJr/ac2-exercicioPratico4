import java.io.*;
import java.util.*;

public class MainCode {
    protected static Scanner sc = new Scanner(System.in);
    protected static Map<String, Integer> mapMnemonicos = new HashMap<>();

    public static void main(String[] args){
        try {
            Scanner inputFile;
            setupMnemonicos();

            System.out.print("Type the name of the root file: ");
            String fileName = sc.next();
            inputFile = new Scanner(new FileReader(fileName));

            //List of the generated hex codes
            List<String> listExpressions = readFile(inputFile);

            //Generates a new file with the same name, but with a .hex extension
            String outputFileName = fileName.substring(0, fileName.lastIndexOf('.')+1) + "hex";
            createResultFile(listExpressions, outputFileName);
            System.out.println("A file called \"" + outputFileName.substring(outputFileName.lastIndexOf("\\")+1) + "\" was generated in the directory: " +  outputFileName.substring(0, outputFileName.lastIndexOf("\\")));

            inputFile.close();
        } catch (Exception e){
            e.printStackTrace();
        }
    }

    public static List<String> readFile(Scanner scanner){
        List<String> hexExpressions = new ArrayList<>();
        Integer aValue = 0;
        Integer bValue = 0;
        String line;

        while(scanner.hasNext()){
            line = scanner.nextLine();
            line = line.replace("\n", "").replace(" ","");                         //Standardizes the lines
            if(!(line.equals("inicio:") || line.equals("fim."))) {                                                  //In case of the content of the line is 'inicio:' or 'fim.', if it's 'fim.' the program finishes
                char charAux = line.charAt(0);
                if (charAux == 'X') {                           //Char 'X' represents the A value
                    char charValue = line.charAt(2);
                    aValue = charValue >= 48 && charValue <= 57 ? (int) charValue - 48 : (int) charValue - 55;      //Transforms the char in a decimal number
                } else if (charAux == 'Y') {
                    char charValue = line.charAt(2);            //Char 'Y' represents the B value
                    bValue = charValue >= 48 && charValue <= 57 ? (int) charValue - 48 : (int) charValue - 55;      //Transforms the char in a decimal number
                } else {                                        //Char 'W' or other one represents the operation
                    String mnemonico = line.substring(2, line.lastIndexOf(';'));                                //Extracts the mnemonic from the line

                    //The next line transforms the decimal representation in a hexadecimal character representation of A and B
                    //It also takes the decimal value of the mnemonic and transforms it in a hexadecimal character representation
                    hexExpressions.add(new StringBuilder().append(toHexa(aValue)).append(toHexa(bValue)).append(toHexa(mapMnemonicos.get(mnemonico))).toString());
                }
            } else if(line.equals("fim.")){
                break;
            }
        }

        return hexExpressions;
    }

    //References the decimal values of each mnemonic sentence
    public static void setupMnemonicos(){
        mapMnemonicos.put("An", 0);
        mapMnemonicos.put("nAoB", 1);
        mapMnemonicos.put("AnB", 2);
        mapMnemonicos.put("zeroL", 3);
        mapMnemonicos.put("nAeB", 4);
        mapMnemonicos.put("Bn", 5);
        mapMnemonicos.put("AxB", 6);
        mapMnemonicos.put("ABn", 7);
        mapMnemonicos.put("AnoB", 8);
        mapMnemonicos.put("nAxB", 9);
        mapMnemonicos.put("copiaB", 10);
        mapMnemonicos.put("AB", 11);
        mapMnemonicos.put("umL", 12);
        mapMnemonicos.put("AoBn", 13);
        mapMnemonicos.put("AoB", 14);
        mapMnemonicos.put("copiaA", 15);
    }

    //Transforms an integer into a hexadecimal character
    public static Character toHexa(Integer  value){
        return (value < 10) ? value.toString().charAt(0) : ((char) (value + 55));
    }

    public static void createResultFile(List<String> result, String fileName) throws IOException{
        BufferedWriter bf;
        try{
            bf = new BufferedWriter(new FileWriter(fileName));
        } catch (Exception e){
            e.printStackTrace();
            return;
        }

        //Write in the file the generated operations
        try {
            result.forEach(expression -> {
                try {
                    bf.write(expression + "\n");
                } catch (Exception e) {
                    e.printStackTrace();
                }
            });
            //The finish case is 'GGG', so it's the last operation in the file
            bf.write("GGG ");
        } catch (Exception e){
            e.printStackTrace();
        }

        bf.close();
    }

}
