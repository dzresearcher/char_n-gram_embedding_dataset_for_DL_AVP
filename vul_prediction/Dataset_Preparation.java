/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package zm.vul_prediction;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.StringTokenizer;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.deeplearning4j.models.fasttext.FastText;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.factory.Nd4j;
import org.deeplearning4j.bagofwords.vectorizer.TfidfVectorizer;
import org.deeplearning4j.text.sentenceiterator.LineSentenceIterator;
import org.deeplearning4j.text.sentenceiterator.SentenceIterator;
import org.deeplearning4j.text.sentenceiterator.SentencePreProcessor;
import org.deeplearning4j.text.tokenization.tokenizer.preprocessor.LowCasePreProcessor;
import org.deeplearning4j.text.tokenization.tokenizerfactory.DefaultTokenizerFactory;
import org.deeplearning4j.text.tokenization.tokenizerfactory.NGramTokenizerFactory;
import org.deeplearning4j.text.tokenization.tokenizerfactory.TokenizerFactory;

/**
 *
 * @author ZM
 */
public class Dataset_Preparation {
    
    
    public List<String> Get_FT_Vectors(String Tokens_File_Name, String Labeled_File_Name, String FT_Model)
    {
        FastText F_T = new FastText();
        String Dataset_Line;
        List<String> Dataset = new ArrayList<>();
        List<String> Filtred_Tokens = new ArrayList<>();
        StringTokenizer Str_Tokenizer;
        INDArray Line_Vector;
        INDArray Token_Vector;
        int Tokens_Number;
        String Temp;
        int Vector_Size;
        
        Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.INFO, "Loading model...");
        F_T.loadBinaryModel(FT_Model);
        Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.INFO, "...OK");
        Vector_Size = F_T.getDimension();
        System.out.println(Vector_Size);
        
        Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.INFO, "Loading tokens...");
        List<String> Lines = new ArrayList<>();
        List<String> Labeled_Lines = new ArrayList<>();
        try {
            Lines = Files.readAllLines(Paths.get(Tokens_File_Name));
            Labeled_Lines = Files.readAllLines(Paths.get(Labeled_File_Name));
        } catch (IOException ex) {
            Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.INFO, "...OK");
        
        
        Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.INFO, "Processing...");
        int Index =0;
        for (String Line : Lines)
        {
            Str_Tokenizer = new StringTokenizer(Line);
            Line_Vector = Nd4j.zeros(Vector_Size);
            Tokens_Number = Str_Tokenizer.countTokens();
            Filtred_Tokens.clear();
            while (Str_Tokenizer.hasMoreTokens())
            {
                //System.out.println(defaultTokenizer.nextToken());
                Temp = Str_Tokenizer.nextToken();
                if (F_T.hasWord(Temp))
                {
                    //Filtred_Tokens.add(Temp);
                    Token_Vector = F_T.getWordVectorMatrixNormalized(Temp);
                }               
                else
                {
                    Token_Vector = Nd4j.zeros(Vector_Size);
                }
                Line_Vector = Line_Vector.add(Token_Vector); 
            }
            Line_Vector = Line_Vector.div(Tokens_Number);
            
            Dataset_Line = "";
            for (int i=0; i<Line_Vector.length(); i++ )
            {
                Dataset_Line = Dataset_Line  + Line_Vector.getFloat(i) + ",";
            }
            
            if ("__label__c".equals(Labeled_Lines.get(Index).substring(0, 10)) )
            {
                Dataset_Line = Dataset_Line + "clean";
            }
            else
            {
                Dataset_Line = Dataset_Line + "vulnerable";
            }
            
            Dataset.add(Dataset_Line);  
            Index++;
        }
        
        Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.INFO, "Saving to file...");

        RandomAccessFile stream;
        try 
        {
            stream = new RandomAccessFile(Tokens_File_Name + ".arff", "rw");
            FileChannel channel = stream.getChannel();
            
            //Dataset header
            String Header;
            Header = "@relation vuldataset\n\n";
            
            for (int i=0; i<Vector_Size; i++)
            {
                Header = Header + "@attribute ve" + i + " numeric\n";
            }
            
            Header = Header + "\n@attribute Status {clean,vulnerable}\n\n";
            Header = Header + "@data\n";
            byte[] strBytes = (Header + "\n").getBytes();
            ByteBuffer buffer = ByteBuffer.allocate(strBytes.length);
            buffer.put(strBytes);
            buffer.flip();
            channel.write(buffer);
            
            for (String Line : Dataset)
            {
                
                strBytes = (Line + "\n").getBytes();
                buffer = ByteBuffer.allocate(strBytes.length);
                buffer.put(strBytes);
                buffer.flip();
                channel.write(buffer);
            }
            buffer.clear();
            stream.close();
            channel.close();
            
            
        } 
        catch (FileNotFoundException ex) 
        {
            Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.SEVERE, null, ex);
        } 
        catch (IOException ex) 
        {
            Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.INFO, "...Done!");
        return Dataset;        
    }
    
    public List<String> Get_DF_IF(String Tokens_File_Name, String Labeled_File_Name, int Vector_Size)
    {
        String Dataset_Line;
        List<String> Dataset = new ArrayList<>();
        List<String> Filtred_Tokens = new ArrayList<>();
        StringTokenizer Str_Tokenizer;
        INDArray Line_Vector;
        float[] Reduced_Line_Vector = new float[Vector_Size];
        //INDArray Token_Vector;
        int Tokens_Number;
        String Temp;
        
        Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.INFO, "Building TF-IDF Vectoriser...");
        TfidfVectorizer TFIDF_Vec = Build_TFIDF_Vec(Tokens_File_Name);
        if (TFIDF_Vec != null)
        {
            Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.INFO, "... OK");
        }
        else
        {
            Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.SEVERE, "...FAILD!!!");
            return null;
        }
             
        Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.INFO, "Loading tokens...");
        List<String> Lines = new ArrayList<>();
        List<String> Labeled_Lines = new ArrayList<>();
        try {
            Lines = Files.readAllLines(Paths.get(Tokens_File_Name));
            Labeled_Lines = Files.readAllLines(Paths.get(Labeled_File_Name));
        } catch (IOException ex) {
            Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.SEVERE, "...FAILD!!!");
            Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.SEVERE, null, ex);
            return null;
        }
        
        Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.INFO, "...OK");
        
        
        Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.INFO, "Processing...");
        int Index =0;
        for (String Line : Lines)
        {
            Str_Tokenizer = new StringTokenizer(Line);
            
            //Line_Vector = Nd4j.zeros(Vector_Size);
            Tokens_Number = Str_Tokenizer.countTokens();
            Filtred_Tokens.clear();
            while (Str_Tokenizer.hasMoreTokens())
            {
                Temp = Str_Tokenizer.nextToken();
                Filtred_Tokens.add(Temp);
            }
            
            /*for (int i=0; i<Filtred_Tokens.size(); i++ )
            {
                Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.INFO, Filtred_Tokens.get(i));
                //System.out.print(Filtred_Tokens.get(i));
                //System.out.print(" ");
            }*/
            Line_Vector = TFIDF_Vec.transform(Filtred_Tokens);
            
            /*double x = Math.ceil(Line_Vector.length() / Vector_Size);
            Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.INFO, "Line_Vector.length : " + Line_Vector.length());
            Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.INFO, "X : " + x);
            Dataset_Line = "";
            
            // reducing vetor size
            double y =0;
            double z=0;
            int r =0;
            int Index2 =0;*/
            for (int i=0;  i< Line_Vector.length() /*(i< Line_Vector.length() && r< Vector_Size)*/; i++ )
            {
                /*if (y<x)
                {
                    z = z + Line_Vector.getFloat(i);
                    y++;
                }
                else
                {
                    z = z / x;
                    Dataset_Line = Dataset_Line  + z + ",";
                    y = 0;
                    z = 0;
                    
                    // continue
                    z = z + Line_Vector.getFloat(i);
                    y++;
                    
                    
                    r++;
                }*/
                
                if (i < Vector_Size )
                {
                    Reduced_Line_Vector[i] = Line_Vector.getFloat(i); 
                }
                else
                {
                    Reduced_Line_Vector[i - ((i/Vector_Size)*Vector_Size)] = Reduced_Line_Vector[i - ((i/Vector_Size)*Vector_Size)] + Line_Vector.getFloat(i);
                }
            }
            Dataset_Line = "";
            for (int i=0;  i< Reduced_Line_Vector.length ; i++ )
            {
                Dataset_Line = Dataset_Line  + Reduced_Line_Vector[i] + ",";
            }
            //Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.INFO, "r : " + r);
            if ("__label__c".equals(Labeled_Lines.get(Index).substring(0, 10)) )
            {
                Dataset_Line = Dataset_Line + "clean";
            }
            else
            {
                Dataset_Line = Dataset_Line + "vulnerable";
            }
            
            Dataset.add(Dataset_Line);  
            Index++;
        }
        
        Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.INFO, "Saving to file...");

        RandomAccessFile stream;
        try 
        {
            stream = new RandomAccessFile(Tokens_File_Name + "_TFIDF.arff", "rw");
            FileChannel channel = stream.getChannel();
            
            //Dataset header
            String Header;
            Header = "@relation vuldataset\n\n";
            
            for (int i=0; i<Vector_Size; i++)
            {
                Header = Header + "@attribute ve" + i + " numeric\n";
            }
            
            Header = Header + "\n@attribute Status {clean,vulnerable}\n\n";
            Header = Header + "@data\n";
            byte[] strBytes = (Header + "\n").getBytes();
            ByteBuffer buffer = ByteBuffer.allocate(strBytes.length);
            buffer.put(strBytes);
            buffer.flip();
            channel.write(buffer);
            
            for (String Line : Dataset)
            {
                
                strBytes = (Line + "\n").getBytes();
                buffer = ByteBuffer.allocate(strBytes.length);
                buffer.put(strBytes);
                buffer.flip();
                channel.write(buffer);
            }
            buffer.clear();
            stream.close();
            channel.close();
            
            
        } 
        catch (FileNotFoundException ex) 
        {
            Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.SEVERE, "...FAILD!!!");
            Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.SEVERE, null, ex);
            return null;
        } 
        catch (IOException ex) 
        {
            Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.SEVERE, "...FAILD!!!");
            Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.SEVERE, null, ex);
            return null;
        }
        
        Logger.getLogger(Dataset_Preparation.class.getName()).log(Level.INFO, "...Done!");
        return Dataset;        
    }
    
    /**
     * Build the TF-IDF vectoriser
     * @param Tokens_File_Name : the source file
     * @return TF-IDF vectoriser
     */
    TfidfVectorizer Build_TFIDF_Vec (String Tokens_File_Name)
    {
        SentenceIterator iter = new LineSentenceIterator(new File(Tokens_File_Name));
        iter.setPreProcessor(new SentencePreProcessor() 
        {
            @Override
            public String preProcess(String sentence) {
                return sentence.toLowerCase();
            }
        });
        
        TokenizerFactory t = new DefaultTokenizerFactory();
        t.setTokenPreProcessor(new LowCasePreProcessor());
        TfidfVectorizer TFIDF_Vec = new TfidfVectorizer.Builder()
                .setMinWordFrequency(5)
                .setStopWords(new ArrayList<String>())
                .setTokenizerFactory(t)
                .setIterator(iter)
                .build();

        TFIDF_Vec.fit();
        
        return TFIDF_Vec;
    }
}
