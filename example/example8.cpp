/*
    example/example8.cpp -- binding classes with custom reference counting,
    implicit conversions between types

    Copyright (c) 2015 Wenzel Jakob <wenzel@inf.ethz.ch>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/

#include "example.h"
#include "object.h"

/// Object subclass
class MyObject : public Object {
public:
    MyObject(int value) : value(value) {
        std::cout << toString() << " constructor" << std::endl;
    }

    std::string toString() const {
        return "MyObject[" + std::to_string(value) + "]";
    }

protected:
    virtual ~MyObject() {
        std::cout << toString() << " destructor" << std::endl;
    }

private:
    int value;
};

class MyObject2 : public std::enable_shared_from_this<MyObject2> {
public:
    MyObject2(int value) : value(value) {
        std::cout << toString() << " constructor" << std::endl;
    }

    std::string toString() const {
        return "MyObject2[" + std::to_string(value) + "]";
    }

    virtual ~MyObject2() {
        std::cout << toString() << " destructor" << std::endl;
    }

private:
    int value;
};

/// Make pybind aware of the ref-counted wrapper type
PYBIND11_DECLARE_HOLDER_TYPE(T, ref<T>);
PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

Object *make_object_1() { return new MyObject(1); }
ref<Object> make_object_2() { return new MyObject(2); }
MyObject *make_myobject_4() { return new MyObject(4); }
ref<MyObject> make_myobject_5() { return new MyObject(5); }

MyObject2 *make_myobject2_1() { return new MyObject2(1); }
std::shared_ptr<MyObject2> make_myobject2_2() { return std::make_shared<MyObject2>(2); }

void print_object_1(const Object *obj) { std::cout << obj->toString() << std::endl; }
void print_object_2(ref<Object> obj) { std::cout << obj->toString() << std::endl; }
void print_object_3(const ref<Object> &obj) { std::cout << obj->toString() << std::endl; }
void print_object_4(const ref<Object> *obj) { std::cout << (*obj)->toString() << std::endl; }

void print_myobject_1(const MyObject *obj) { std::cout << obj->toString() << std::endl; }
void print_myobject_2(ref<MyObject> obj) { std::cout << obj->toString() << std::endl; }
void print_myobject_3(const ref<MyObject> &obj) { std::cout << obj->toString() << std::endl; }
void print_myobject_4(const ref<MyObject> *obj) { std::cout << (*obj)->toString() << std::endl; }

void print_myobject2_1(const MyObject2 *obj) { std::cout << obj->toString() << std::endl; }
void print_myobject2_2(std::shared_ptr<MyObject2> obj) { std::cout << obj->toString() << std::endl; }
void print_myobject2_3(const std::shared_ptr<MyObject2> &obj) { std::cout << obj->toString() << std::endl; }
void print_myobject2_4(const std::shared_ptr<MyObject2> *obj) { std::cout << (*obj)->toString() << std::endl; }

void init_ex8(py::module &m) {
    py::class_<Object, ref<Object>> obj(m, "Object");
    obj.def("getRefCount", &Object::getRefCount);

    py::class_<MyObject, ref<MyObject>>(m, "MyObject", obj)
        .def(py::init<int>());

    m.def("make_object_1", &make_object_1);
    m.def("make_object_2", &make_object_2);
    m.def("make_myobject_4", &make_myobject_4);
    m.def("make_myobject_5", &make_myobject_5);
    m.def("print_object_1", &print_object_1);
    m.def("print_object_2", &print_object_2);
    m.def("print_object_3", &print_object_3);
    m.def("print_object_4", &print_object_4);
    m.def("print_myobject_1", &print_myobject_1);
    m.def("print_myobject_2", &print_myobject_2);
    m.def("print_myobject_3", &print_myobject_3);
    m.def("print_myobject_4", &print_myobject_4);

    py::class_<MyObject2, std::shared_ptr<MyObject2>>(m, "MyObject2")
        .def(py::init<int>());
    m.def("make_myobject2_1", &make_myobject2_1);
    m.def("make_myobject2_2", &make_myobject2_2);
    m.def("print_myobject2_1", &print_myobject2_1);
    m.def("print_myobject2_2", &print_myobject2_2);
    m.def("print_myobject2_3", &print_myobject2_3);
    m.def("print_myobject2_4", &print_myobject2_4);

    py::implicitly_convertible<py::int_, MyObject>();
}
