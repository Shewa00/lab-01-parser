#include "my_json.hpp"

Json::Json() {}

Json::Json(const std::vector<std::any>& v) {
    vec.field_arr = v;
}

Json::Json(const std::string & s) {
    std::string::const_iterator it =s.cbegin();
    std::string key = "", value1 = "";
    std::any value;
    while (*it != '\n') {
        if (*it == '"') {
            ++it;
            while (*it != '"') {
                key += *it;
                ++it;
            }
        }
        if (*it == ':') {
            ++it;
            while (*it == ' ') {
                ++it;
            }
            if (*it == '"') {
                ++it;
                while (*it != '"') {
                    value1 += *it;
                    ++it;
                }
                object.field_obj.insert(std::pair<std::string,
                        std::string>(key, value1));
                key = value1 = "";
            } else {
                if (*it == '[') {
                    std::vector<std::any> v;
                    while (*it != ']') {
                        ++it;
                        while (*it == ' ') {
                            ++it;
                        }
                        if (isdigit(*it)) {
                            v.push_back(atof(&(*it)));
                            while (*it != ',' && *it != ']') {
                                ++it;
                            }
                        } else {
                            if (*it == '"') {
                                ++it;
                                std::string s_arr = "";
                                while (*it != '"') {
                                    s_arr += *it;
                                    ++it;
                                }
                                v.push_back(s_arr);
                            } else {
                                if (*it == '[') {
                                    ++it;
                                    std::string s_mas = "";
                                    while (*it != ']'){
                                        s_mas += *it;
                                        ++it;
                                    }
                                    s_mas += " ";
                                    ++it;
                                    v.push_back(Json(s_mas));
                                } else {
                                    if (*it == '{') {
                                        ++it;
                                        std::string str = "";
                                        while (*it != '}'){
                                            str += *it;
                                            ++it;
                                        }
                                        v.push_back(Json(str));
                                    }
                                }
                            }
                        }
                    }
                    Json temp_arr(v);
                    value = temp_arr;
                    object.field_obj.insert(std::pair<std::string,
                            std::any>(key, value));
                } else {
                    if (*it == '{') {
                        std::string temp_s;
                        while (*it != '}') {
                            temp_s += *it;
                            ++it;
                        }
                        temp_s += '}';
                        Json temp(temp_s);
                        object.field_obj.insert(
                                std::pair<std::string, Json>(key, temp));
                    } else {
                        if (isdigit(*it)) {
                            value = atof(&(*it));
                            object.field_obj.insert(
                                    std::pair<std::string, std::any>(key, value));
                        } else {
                            if (*it == 'f') {
                                std::string s_bool = "";
                                while (*it != ',' && *it != '"' && *it != ' ') {
                                    s_bool += *it;
                                    ++it;
                                }
                                if (s_bool == "false") {
                                    value = false;
                                    object.field_obj.insert(
                                            std::pair<std::string, std::any>(key, value));
                                }
                            } else {
                                if (*it == 't') {
                                    std::string s_bool = "";
                                    while (*it != ',') {
                                        s_bool += *it;
                                        ++it;
                                    }
                                    if (s_bool == "true") {
                                        value = true;
                                        object.field_obj.insert(
                                                std::pair<std::string, std::any>(key, value));
                                    }
                                } else {
                                    if (*it == 'n') {
                                        std::string s_bool = "";
                                        while (*it != ',' && *it != ' ' && *it != '"') {
                                            s_bool += *it;
                                            ++it;
                                        }
                                        if (s_bool == "null") {
                                            value = NULL;
                                            object.field_obj.insert(
                                                    std::pair<std::string, std::any>(key, value));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                key = "";
            }
        }
        ++it;
    }

}

bool Json::is_array() const {
    return !vec.field_arr.empty();
}

bool Json::is_object() const{
    return !object.field_obj.empty();
}

std::any & Json::operator[](const std::string & key) {
    if (this->is_object()) {
        for (auto it = object.field_obj.begin();
             it != object.field_obj.end(); ++it) {
            if (it->first == key) {
                return it->second;
            }
        }
    }
    throw std::runtime_error("it`s not an object");
}

std::any & Json::operator[](size_t index) {
    if (this->is_array()) {
        if (index < vec.field_arr.size()) {
            return this->vec.field_arr[index];
        }
    }
    throw std::runtime_error("it`s not an array");
}

Json Json::parse(const std::string & s) {
    return Json(s);
}

Json Json::parseFile(const std::string & path_to_file) {
    std::ifstream fin(path_to_file);
    char temp;
    std::string s;
    if (fin.is_open()) {
        while (!fin.eof()) {
            fin >> temp;
            s += temp;
        }
    } else {
        std::cout << "Данного файла не существует." << std::endl;
    }
    fin.close();
    return Json(s);
}
