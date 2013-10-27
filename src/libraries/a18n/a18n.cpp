#include "a18n.h"
#include "aJSON.h"

a18n::a18n() {}

a18n::a18n(char* data) {
    setData(data);
}

void a18n::setData(char* data) {
    this->data = data;
    /*this->data = new char(strlen(data) + 1);
    strcpy(this->data, data);*/
}

char* a18n::getTranslation(char* code, char* item) {
    aJsonObject* root = aJson.parse(this->data);
    if (root != NULL) {
        aJsonObject* jitem = aJson.getObjectItem(root, item);
        if (item != NULL) {
            aJsonObject* jtranslation = aJson.getObjectItem(jitem, code);
            unsigned int len = strlen(jtranslation->valuestring) + 1;
            char* translation = new char(len);
            strcpy(translation, jtranslation->valuestring);
            //aJson.deleteItem(root);
            return translation;
        }
    }
}

a18n::~a18n() {
    delete(this->data);
}
