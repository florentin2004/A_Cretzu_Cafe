#include "Logger.h"

void Logger::logAction(std::string action)
{
	std::ofstream fout("Logger.txt", std::ios::app);
	if (!fout.is_open())
	{
		std::cerr << "[EROARE] Deschiderea fisierului de log a esuat!";
		return;
	}

	fout << action << "\n";

	fout.close();
}
