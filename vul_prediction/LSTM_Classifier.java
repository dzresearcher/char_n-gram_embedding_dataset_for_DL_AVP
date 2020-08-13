/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


package zm.vul_prediction;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import weka.classifiers.Evaluation;
import weka.classifiers.functions.Dl4jMlpClassifier;
import weka.core.Debug.Random;
import weka.core.Instances;
import weka.dl4j.GradientNormalization;
import weka.dl4j.NeuralNetConfiguration;
import weka.dl4j.activations.ActivationReLU;
import weka.dl4j.activations.ActivationSoftmax;
import weka.dl4j.activations.ActivationTanH;
import weka.dl4j.earlystopping.EarlyStopping;
import weka.dl4j.layers.DenseLayer;
import weka.dl4j.layers.OutputLayer;

/**
 *
 * @author ZM
 */
public class LSTM_Classifier {
    
    public Dl4jMlpClassifier LSTM_Clf = null;
    
    String Build_Evaluate_LSTM(String Dataset_File_Name, String Options, int Num_Folds, boolean Balance_Dataset)
    {
        StringBuffer Results = new StringBuffer();
        
        // get options
        /* option form : L1:0.01 L2:0.01 N:1000 DL:64 LSTML:128,64 
        L1 : learning rate, L2 : l2, N: num epoches , DL: num neurone of dense layer, LSTML : lstm layer (first with 128 neuron, second with 64,..)
        */
        String[] Opts = Options.split(" ");
        // L1
        double L1 =  Double.parseDouble(Opts[0].split(":")[1]);
        // L2
        double L2 =  Double.parseDouble(Opts[1].split(":")[1]);
        // N
        int N =  Integer.parseInt(Opts[2].split(":")[1]);
        //DL (number of neurone in dense layer)
        int DL = Integer.parseInt(Opts[3].split(":")[1]);
        // LSTM Layers
        String[] Lstm_L = (Opts[4].split(":")[1]).split(",");
        
        LSTM_Clf = new Dl4jMlpClassifier();
        
        // Setup hyperparameters
        final int seed = 1;
        final double gradientThreshold = 1.0;
 
        // Initialize the classifier
        LSTM_Clf.setSeed(seed);
        LSTM_Clf.setNumEpochs(N);
       
        // Network config
        NeuralNetConfiguration nnc = new NeuralNetConfiguration();
        nnc.setL2(L2);
        nnc.setGradientNormalization(GradientNormalization.ClipElementWiseAbsoluteValue);
        nnc.setGradientNormalizationThreshold(gradientThreshold);
        nnc.setL1(L1); //setLearningRate(learningRate);

        // Config classifier
        //LSTM_Clf.setLayers(LSTM_Layer, Dense_Layer, Output_Layer);
        LSTM_Clf.setNeuralNetConfiguration(nnc);
        //LSTM_Clf.setEarlyStopping(new EarlyStopping(5, 20));
        
        // Define the layers
        // dense layer
        DenseLayer Dense_Layer1 = new DenseLayer();
        Dense_Layer1.setNOut(DL);
        Dense_Layer1.setActivationFunction(new ActivationReLU());
        
        /*DenseLayer Dense_Layer2 = new DenseLayer();
        Dense_Layer2.setNOut(16);
        Dense_Layer2.setActivationFunction(new ActivationReLU());
        
        DenseLayer Dense_Layer3 = new DenseLayer();
        Dense_Layer3.setNOut(8);
        Dense_Layer3.setActivationFunction(new ActivationReLU());*/


        OutputLayer Output_Layer = new OutputLayer();
        Output_Layer.setActivationFunction(new ActivationSoftmax());
        //Output_Layer.setNOut(2);
        

        //lstm layer
        if(Lstm_L.length == 1)
        {
            int Num_Neurones = Integer.parseInt(Lstm_L[0]);
            weka.dl4j.layers.LSTM LSTM_Layer = new weka.dl4j.layers.LSTM();
            LSTM_Layer.setNOut(Num_Neurones);
            LSTM_Layer.setActivationFunction(new ActivationTanH());
            
            LSTM_Clf.setLayers(LSTM_Layer, Dense_Layer1/*, Dense_Layer2, Dense_Layer3*/, Output_Layer);
            
        }
        else if (Lstm_L.length == 2)
        {
            int Num_Neurones1 = Integer.parseInt(Lstm_L[0]);
            int Num_Neurones2 = Integer.parseInt(Lstm_L[1]);
            weka.dl4j.layers.LSTM LSTM_Layer1 = new weka.dl4j.layers.LSTM();
            LSTM_Layer1.setNOut(Num_Neurones1);
            LSTM_Layer1.setActivationFunction(new ActivationTanH());
            
            weka.dl4j.layers.LSTM LSTM_Layer2 = new weka.dl4j.layers.LSTM();
            LSTM_Layer2.setNOut(Num_Neurones2);
            LSTM_Layer2.setActivationFunction(new ActivationTanH());
            
            LSTM_Clf.setLayers(LSTM_Layer1, LSTM_Layer2, Dense_Layer1/*, Dense_Layer2, Dense_Layer3*/, Output_Layer);
        
        }
        else if (Lstm_L.length == 3)
        {
            int Num_Neurones1 = Integer.parseInt(Lstm_L[0]);
            int Num_Neurones2 = Integer.parseInt(Lstm_L[1]);
            int Num_Neurones3 = Integer.parseInt(Lstm_L[2]);
            weka.dl4j.layers.LSTM LSTM_Layer1 = new weka.dl4j.layers.LSTM();
            LSTM_Layer1.setNOut(Num_Neurones1);
            LSTM_Layer1.setActivationFunction(new ActivationTanH());
            
            weka.dl4j.layers.LSTM LSTM_Layer2 = new weka.dl4j.layers.LSTM();
            LSTM_Layer2.setNOut(Num_Neurones2);
            LSTM_Layer2.setActivationFunction(new ActivationTanH());
            
            weka.dl4j.layers.LSTM LSTM_Layer3 = new weka.dl4j.layers.LSTM();
            LSTM_Layer3.setNOut(Num_Neurones3);
            LSTM_Layer3.setActivationFunction(new ActivationTanH());
            
            LSTM_Clf.setLayers(LSTM_Layer1, LSTM_Layer2, LSTM_Layer3, Dense_Layer1/*, Dense_Layer2, Dense_Layer3*/, Output_Layer);
        
        }else if (Lstm_L.length > 3)
        {
            Logger.getLogger(DL_JFrame.class.getName()).log(Level.SEVERE, "LSTM layers must be <= 3 !!!");
            return null;
        }
        
        
        
        //Train and evaluate
        Instances Train_Data/*, Test_Data*/;
        try 
        {
            Train_Data = new Instances(new FileReader(Dataset_File_Name));
            Train_Data.setClassIndex(Train_Data.numAttributes() - 1);
            /*Test_Data = new Instances(new FileReader(Dataset_File_Name));
            Test_Data.setClassIndex(1);*/
            LSTM_Clf.buildClassifier(Train_Data);
            
            //Evaluation Eval = new Evaluation(Test_Data);
            //evaluateModel(LSTM_Clf, Test_Data);
            Evaluation Eval = new Evaluation(Train_Data);
            Eval.crossValidateModel(LSTM_Clf, Train_Data, Num_Folds, new Random(0));
            
            Results.append(Eval.toSummaryString()).append("\n");
            Results.append(Eval.toMatrixString()).append("\n");
            Results.append(Eval.toClassDetailsString()).append("\n");
            Results.append("False Negative Rate : ").append(Eval.weightedFalseNegativeRate()).append("\n");
            Results.append("False Positive Rate : ").append(Eval.weightedFalsePositiveRate()).append("\n");
            Results.append("FMeasure : ").append(Eval.weightedFMeasure()).append("\n");
            Results.append("AreaUnderROC : ").append(Eval.weightedAreaUnderROC()).append("\n");
        } 
        catch (FileNotFoundException ex) 
        {
            Logger.getLogger(DL_JFrame.class.getName()).log(Level.SEVERE, null, ex);
        } 
        catch (IOException ex) 
        {
            Logger.getLogger(DL_JFrame.class.getName()).log(Level.SEVERE, null, ex);
        } 
        catch (Exception ex) 
        {
            Logger.getLogger(DL_JFrame.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        return Results.toString();
    }
}
