/*
 * File:   TabelaHash.h
 * Author: Daniel Franklin
 *
 * Created on 2018
 * Part of the DaniHash project
 */

#ifndef TABELAHASH_H_INCLUDED
#define TABELAHASH_H_INCLUDED

#include "EstruturaAuxiliar.h"

#include "utilDesenho.h"
#include "utilBenchmark.h"
#include "utilArquivos.h"

class TabelaHash{

    public:

    virtual void inserir(int valor, bool PI)=0;
    virtual void remover(int valor)=0;
    virtual int buscar(int valor, bool PI)=0;
    virtual void imprimir()=0;
    virtual void preparar_janela()=0;
    virtual Results benchmarkINSERCAO(int qtdInsercoes, int opcao, string nomeDoArquivoINS)=0;
    virtual double benchmarkBUSCA(int opcao, string nomeDoArquivoINS)=0;
    virtual float getFC()=0;
    virtual int getColisoesDaInsercaoAtual()=0;
    virtual bool getFezRehashing()=0;

    Results inserirDeArquivo(string nomeDoArquivo){

        int numeroNaLinha, numeroDaLinha = 1;
        string linha, substringINS;
        ifstream fileREAD;
        fileREAD.open(FILEPATH_INS+nomeDoArquivo);
        Results resultado = inicializaResults();

        while(fileREAD){
            getline(fileREAD, linha);
            substringINS = linha.substr(0,4);

            if(substringINS =="INS "){
                try{
                    numeroNaLinha = stoi(linha.substr(4,linha.size()-4));
                }catch(invalid_argument &e){
                    throw arquivo_defeituoso(numeroDaLinha,1);
                }

                benchmark b;
                this->inserir(numeroNaLinha, false);
                resultado.tempoBMK += b.elapsed();

                resultado.colisoes += getColisoesDaInsercaoAtual();

                if (this->getFezRehashing() == true)
                    resultado.rehashings++;

                numeroDaLinha++;

            }else if (linha == ""){
                break;
            }else{
                throw arquivo_defeituoso(numeroDaLinha,2);
            }
        }
        fileREAD.close();
        return resultado;
    }

    virtual ~TabelaHash(){}

};

struct Atributos{

    public:

    int tipo, tamanho, limite;
};

Atributos pegaAtributosDaHash(bool trueBASIC_falseBMK){
    int type;
    Atributos atr;

    cout<<"Que tipo de hash?\n\n";
    cout<<"1) Open Hashing com Lista\n";
    cout<<"2) Open Hashing com Arvore\n";
    cout<<"3) Closed Hashing\n";
    cout<<"4) Half-Open Hashing\n\n";

    type = pegaRespostaMinMax("Opcao: ",1,4);

    if (type == 3){

        system("cls");
        cout<<"Escolha como lidar com colisoes:\n\n";
        cout<<"3) Tentativa linear\n";
        cout<<"4) Tentativa quadratica\n";
        cout<<"5) Duplo hashing\n\n";

        type = pegaRespostaMinMax("Opcao: ",3,5);
    }else if (type == 4){

        system("cls");
        cout<<"Deseja que a hash utilize tentativa quadratica?\n\n";
        cout<<"6) Sim, insercoes em arvores cheias contam como colisoes\n";
        cout<<"7) Nao, pode continuar inserindo em arvores cheias\n\n";

        type = pegaRespostaMinMax("Opcao: ",6,7);
    }

    atr.tipo = type;
    atr.limite = 1; //so pra deixar inicializado com algum valor

    if (trueBASIC_falseBMK == true){
        atr.tamanho = pegaRespostaMin("\nTamanho da tabela: ", 1);

        if (type >= 6)
            atr.limite = pegaRespostaMin("Altura maxima das arvores: ", 1);

    }else{
        atr.tamanho = TAM_INICIAL_HASH_BMK;
        atr.limite = ALT_MAX_BMK;
    }

    return atr;

}

#include "OpenHashing.h"
#include "ClosedHashing.h"
#include "HalfOpenHashing.h"

TabelaHash* instanciaHash(Atributos attr){

    if (attr.tipo <= 2)
        return new Ohash(attr.tamanho,attr.tipo);
    else if (attr.tipo <= 5)
        return new Chash(attr.tamanho,attr.tipo);
    else
        return new HOhash(attr.tamanho,attr.tipo,attr.limite);

}

//
#endif // TABELAHASH_H_INCLUDED
