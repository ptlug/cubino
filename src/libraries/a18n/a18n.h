#ifndef A18N_HH
#define A18N_HH

class a18n {

    private:
        char* data;

    public:
        a18n();
        a18n(char* data);
        ~a18n();
	    void setData(char* data);
        char* getTranslation(char* code, char* item);
};

#endif
