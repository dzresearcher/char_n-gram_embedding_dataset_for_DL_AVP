/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package zm.vul_prediction;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import weka.classifiers.Evaluation;
import weka.classifiers.functions.RnnSequenceClassifier;
import weka.core.Instances;
import weka.dl4j.GradientNormalization;
import weka.dl4j.NeuralNetConfiguration;
import weka.dl4j.activations.ActivationTanH;
import weka.dl4j.iterators.instance.sequence.text.rnn.RnnTextEmbeddingInstanceIterator;
import weka.dl4j.layers.LSTM;
import weka.dl4j.layers.RnnOutputLayer;

/**
 *
 * @author ZM
 */
public class RNN_Classifier {
    
    public String Build_Clf ()
    {
        final File Word_Vectors = new File("E:\\ZM\\Research\\Tools\\fasttext-win64-latest-Release\\Release\\dddc.bin");
        StringBuffer Results = new StringBuffer();
        
        // Setup hyperparameters
        final int truncateLength = 50;
        final int batchSize = 64;
        final int seed = 1;
        final int numEpochs = 1;
        final int tbpttLength = 20;
        final double l2 = 0.1;//1e-5;
        final double gradientThreshold = 1.0;
        final double learningRate = 0.1;//0.01; //0.02;

        // Setup the iterator
        RnnTextEmbeddingInstanceIterator tii = new RnnTextEmbeddingInstanceIterator();
        tii.setWordVectorLocation(Word_Vectors);
        tii.setTruncateLength(truncateLength);
        tii.setTrainBatchSize(batchSize);
        
        

        // Initialize the classifier
        RnnSequenceClassifier clf = new RnnSequenceClassifier();
        clf.setSeed(seed);
        clf.setNumEpochs(numEpochs);
        clf.setInstanceIterator(tii);
        clf.settBPTTbackwardLength(tbpttLength);
        clf.settBPTTforwardLength(tbpttLength);

        // Define the layers
        LSTM lstm = new LSTM();
        lstm.setNOut(128);
        lstm.setActivationFunction(new ActivationTanH());

        RnnOutputLayer rnnOut = new RnnOutputLayer();

        // Network config
        NeuralNetConfiguration nnc = new NeuralNetConfiguration();
        nnc.setL2(l2);
        nnc.setGradientNormalization(GradientNormalization.ClipElementWiseAbsoluteValue);
        nnc.setGradientNormalizationThreshold(gradientThreshold);
        nnc.setL1(learningRate); //setLearningRate(learningRate);

        // Config classifier
        clf.setLayers(lstm, rnnOut);
        clf.setNeuralNetConfiguration(nnc);
        Instances data, test_data;
        try {
            data = new Instances(new FileReader("E:\\ZM\\Research\\Colaborative_Works\\Dr_Mamdouh\\Features_Learning_For_Deep_Vul_Detec\\Experiments\\Data_version2\\Final_Dataset\\DDCD\\Lower_case_Code_gadgets_DDCD_for_training.txt_Labeled_Tokens.arff"));
            data.setClassIndex(1);
            test_data = new Instances(new FileReader("E:\\ZM\\Research\\Colaborative_Works\\Dr_Mamdouh\\Features_Learning_For_Deep_Vul_Detec\\Experiments\\Data_version2\\Final_Dataset\\DDCD\\Lower_case_Code_gadgets_DDCD_for_testing.txt_Labeled_Tokens.arff"));
            test_data.setClassIndex(1);
        clf.buildClassifier(data);
        
        Evaluation Eval = new Evaluation(test_data);
        Eval./*crossValidateModel(clf, data, 3, new Debug.Random(1)); //*/evaluateModel(clf, test_data); //
                
        Results.append(Eval.toSummaryString()).append("\n");
        Results.append(Eval.toMatrixString()).append("\n");
        Results.append(Eval.toClassDetailsString()).append("\n");
        Results.append("False Negative Rate : ").append(Eval.weightedFalseNegativeRate()).append("\n");
        Results.append("False Positive Rate : ").append(Eval.weightedFalsePositiveRate()).append("\n");
        Results.append("FMeasure : ").append(Eval.weightedFMeasure()).append("\n");
        Results.append("AreaUnderROC : ").append(Eval.weightedAreaUnderROC()).append("\n");
        
        
       
        } catch (FileNotFoundException ex) {
            Logger.getLogger(DL_JFrame.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(DL_JFrame.class.getName()).log(Level.SEVERE, null, ex);
        } catch (Exception ex) {
            Logger.getLogger(DL_JFrame.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        return Results.toString();
    }
    
}
