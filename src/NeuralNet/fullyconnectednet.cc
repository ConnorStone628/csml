
#include "fullyconnectednet.hh"

namespace CSML{
  
  ////////////////////////////////////////////////////////////////////////////////
  fullyconnectednet::fullyconnectednet(const std::vector<unsigned int> nodes_per_layer){

    this->learning_rate = 1;

    // Add the nodes
    this->SetShape(nodes_per_layer);

    // Add synapses
    for (unsigned int i = 1; i < this->nodes.size(); ++i){
      for (unsigned int n = 0; n < this->nodes[i].size(); ++ n){
	this->AddSynapse(this->bias_node, this->nodes[i][n]);// Synapse with the bias node
	for (unsigned int s = 0; s < this->nodes[i-1].size(); ++s){
	  this->AddSynapse(this->nodes[i-1][s], this->nodes[i][n]);// Add the synapses to the previous layer
	}
      }
    }

  }

  ////////////////////////////////////////////////////////////////////////////////
  fullyconnectednet::~fullyconnectednet(){}

  ////////////////////////////////////////////////////////////////////////////////
  void fullyconnectednet::TopErrors(const std::vector<double> true_values){

    int final_layer = this->nodes.size() - 1; // last layer in the net
  
    // Calculate the error at the output nodes
    for (unsigned int i = 0; i < this->nodes[final_layer].size(); ++i){
      this->nodes[final_layer][i]->source_dendrite->SetDelta((*this->loss_derivative)(true_values[i], *this->nodes[final_layer][i]->output_signal)*this->nodes[final_layer][i]->ActivationD());
    }
  
  }

  ////////////////////////////////////////////////////////////////////////////////
  void fullyconnectednet::BackPropogateErrors(){

    // Backpropogate the error through the net
    for (unsigned int i = this->nodes.size() - 2; i > 0; --i){
      for (unsigned int n = 0; n < this->nodes[i].size(); ++n){
	this->nodes[i][n]->BackPropogate();
      }
    }
  
  }

  void fullyconnectednet::Update(){

    // Tell each dendrite to update its weights
    for (unsigned int i = 0; i < this->nodes.size(); ++i){
      for (unsigned int n = 0; n < this->nodes[i].size(); ++n){
	this->nodes[i][n]->source_dendrite->Update();
      }
    }
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void fullyconnectednet::Learn(data* _X){

    for (unsigned int i = 0; i < this->itterations; ++i){
      for (unsigned int f = 0; f < _X->GetSize(); ++f){
	this->Reset("i");
	this->Propogate(*_X->GetElement(f));
	this->TopErrors(*_X->GetLabel(f));
	this->BackPropogateErrors();
	this->Update();
      }
    }
    
  }

  ////////////////////////////////////////////////////////////////////////////////
  void fullyconnectednet::Predict(data* _X){
    
    for (unsigned int i = 0; i < _X->GetSize(); ++i){
	this->Reset("i");
	this->Propogate(*_X->GetElement(i));
	_X->SetHypothesis(i, this->Output());
    }

  }

  ////////////////////////////////////////////////////////////////////////////////
  void fullyconnectednet::SetParam(const std::string param, const double value){
    if (param == "learningrate" && value > 0){
      this->learning_rate = value;
      for (unsigned int i = 0; i < this->nodes.size(); ++i){
	for (unsigned int d = 0; d < this->nodes[i].size(); ++d){
	  this->nodes[i][d]->source_dendrite->SetLearningRate(value);
	}
      }
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  void fullyconnectednet::SetParam(const std::string param, const int value){
    if (param == "itterations" && value > 0){
      this->itterations = value;
    }
  }
  
}
