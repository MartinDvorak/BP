// FunkceMakularniTepenky.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//
#include "pch.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include "vascular.h"
#include "adapter.h"

int main()
{
	
	vascular* UperiorMacularArterioly = new vascular(new vascularPoint(0.0, 0.0));

	UperiorMacularArterioly->makeTreeStructure();
	UperiorMacularArterioly->printTreeSructure();

	bifurcationPoint* newTree = nullptr;
	adapter* newInterface = new adapter(UperiorMacularArterioly,newTree);
	newInterface->controlPassageTree();
	newInterface->printBifurcationTree();


	//std::ofstream myfile;
	//myfile.open("example.txt");
	//double x, y;
	//do{
	//	UperiolMacularArterioly->getCoords(&x,&y);
	//	myfile << x << "\t" << y << "\n";
	//	UperiolMacularArterioly->doStep();
	//} while (x < 80.0);
	//myfile.close();
}

// Spuštění programu: Ctrl+F5 nebo nabídka Ladit > Spustit bez ladění
// Ladění programu: F5 nebo nabídka Ladit > Spustit ladění

// Tipy pro zahájení práce:
//   1. K přidání nebo správě souborů použijte okno Průzkumník řešení.
//   2. Pro připojení ke správě zdrojového kódu použijte okno Team Explorer.
//   3. K zobrazení výstupu sestavení a dalších zpráv použijte okno Výstup.
//   4. K zobrazení chyb použijte okno Seznam chyb.
//   5. Pokud chcete vytvořit nové soubory kódu, přejděte na Projekt > Přidat novou položku. Pokud chcete přidat do projektu existující soubory kódu, přejděte na Projekt > Přidat existující položku.
//   6. Pokud budete chtít v budoucnu znovu otevřít tento projekt, přejděte na Soubor > Otevřít > Projekt a vyberte příslušný soubor .sln.
