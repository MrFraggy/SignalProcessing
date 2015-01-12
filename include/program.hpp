#pragma once

class Program {
public:
	Program(const std::string& prog) : f(nullptr)
	{
		f = popen(prog.c_str(), "w");
	}
	~Program()
	{
		if(f)
			fclose(f);
	}
	void command(const std::string& str)
	{
		std::string cmd = str+"\n";
		fprintf(f, cmd.c_str());
	}

	bool isValid()
	{
		return f != nullptr;
	}

	void flush()
	{
		fflush(f);
	}
protected:
	FILE* f;
};
