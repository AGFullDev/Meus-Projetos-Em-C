================================================================================

                                  PARKCONTROL

                     Sistema de Estacionamento Inteligente

================================================================================



PROJETO: Trabalho Prático - Programação e Desenvolvimento de Software 2 (PDS2)

SEMESTRE: 2026/1

TURMA/GRUPO: Metaturma - Grupo 10

REPOSITÓRIO: 2026-1-METATURMA-grupo10

PROFESSORES: Douglas Guimarães Macharet e Luiz Chaimowicz

DATA DE APRESENTAÇÃO: 18 de junho de 2026



--------------------------------------------------------------------------------

1. EQUIPE DE DESENVOLVIMENTO

--------------------------------------------------------------------------------

- Guilherme Mateus Gonçalves

- Luis Gustavo Pereira de Souza

- Matheus Felipe Silva Serra

- Tamires Sousa de Melo

(E você!)



--------------------------------------------------------------------------------

2. SOBRE O PROJETO

--------------------------------------------------------------------------------

O ParkControl é um sistema desenvolvido em C++ com o objetivo de automatizar e

otimizar o gerenciamento de estacionamentos. A plataforma centraliza o controle

de fluxo de veículos, alocação inteligente de vagas, cálculo dinâmico de 

tarifas, processamento de pagamentos e geração de relatórios gerenciais.



--------------------------------------------------------------------------------

3. PRINCIPAIS FUNCIONALIDADES

--------------------------------------------------------------------------------

* Gestão de Veículos e Vagas:

  - Cadastro e categorização de veículos (Carro, Moto, Caminhão).

  - Gerenciamento de vagas por setores e tipos (Comum, Especial/Reservada).

  - Verificação de disponibilidade em tempo real.



* Controle de Fluxo e Cobrança:

  - Registro automatizado de entrada e saída de veículos.

  - Cálculo de tarifas baseado no tempo de permanência, categoria do veículo 

    e tipo de vaga ocupada.

  - Suporte a múltiplos métodos de pagamento (Crédito, Débito e Pix).



* Administração e Relatórios:

  - Geração de relatórios financeiros (receitas e lucros por período).

  - Análise de rotatividade e preferência de ocupação das vagas.

  - Sistema de registro de ocorrências e observações (avarias, alertas ou 

    suspeitas de fraude associadas a placas/veículos).



--------------------------------------------------------------------------------

4. ESTRUTURA E BOAS PRÁTICAS

--------------------------------------------------------------------------------

- Linguagem: C++ (Paradigma Orientado a Objetos).

- Modelagem: Estruturado a partir de User Stories e cartões CRC 

  (Classe-Responsabilidade-Colaboração).

- Arquitetura modularizada separando interfaces (.hpp) e implementações (.cpp).



--------------------------------------------------------------------------------

5. COMO COMPILAR E EXECUTAR (PADRÃO)

--------------------------------------------------------------------------------

1. Clone o repositório:

   git clone <url-do-repositorio-2026-1-METATURMA-grupo10>



2. Compile o projeto utilizando o Makefile (ou g++):

   make

   # Ou manualmente: g++ -std=c++17 -Iinclude src/*.cpp main.cpp -o parkcontrol



3. Execute o programa:

   ./parkcontrol

================================================================================
