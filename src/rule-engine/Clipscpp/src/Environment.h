/***************************************************************************
 *  Environment.h - Environment Header
 *
 *  Created: 2018-06-04 16:16:50
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __Environment_H__
#define __Environment_H__

#include "ClipsObject.h"
#include "Utility.h"
#include "Fact.h"
#include "Instance.h"
#include "DefaultFacts.h"
#include "Global.h"
#include "Module.h"
#include "Rule.h"
#include "Function.h"
#include "Class.h"
#include "Any.h"

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <cstdio>

extern "C"
int EnvDefineFunction2WithContext(void *, const char *, int, int (*) (void *), const char *, const char *, void *);

#define GLOBAL_SAVE   0
#define LOCAL_SAVE    1
#define VISIBLE_SAVE  2

namespace CLIPS {

template<typename R = void, typename... Args>
class Functor {
public:
    Functor(std::function<R(Args...)> fun) : _fun(fun) {
    }
    template<typename Object>
    Functor(Object* object, R (Object::*method)(Args...))
        : _fun([object, method](Args... args){ return (*object.*method)(args...);}) {
    }
    R operator()(Args... args) {
        return _fun(args...);
    }
private:
    std::function<R(Args...) > _fun;
}; /* class Functor */

class EnvironmentCallback {
public:
    virtual ~EnvironmentCallback() {}
    virtual void onCallClear() = 0;
    virtual void onCallReset() = 0;
    virtual void onPeriodic() = 0;
    virtual void onRuleFiring() = 0;
}; /* class EnvironmentCallback */

class Environment : public ClipsObject {
public:
    Environment();
    ~Environment();

    typedef int (*UserFunc_t)(void*);

    static void s_clear_callback(void *env);
    static void s_reset_callback(void *env);
    static void s_periodic_callback(void *env);
    static void s_rulefiring_callback(void *env);

    void setCallback(EnvironmentCallback *cb) { mCallback = cb; }

/*{{{ general --> */
    int load(const std::string &filename);
    bool save(const std::string &filename);
    bool binary_load(const std::string &filename);
    bool binary_save(const std::string &filename);

    bool build(const std::string &construct);
    bool batch_evaluate(const std::string &filename);
    void reset();

    bool get_halt_execution();
    void set_halt_execution(bool val);
    bool get_halt_rules();
    void set_halt_rules(bool val);
    bool get_evaluation_error();
    void set_evaluation_error(bool val);
/* <-- general }}}*/

/*{{{ debug --> */
    int is_watched(const std::string &item);
    bool watch(const std::string& item);
    bool unwatch(const std::string& item);
    bool is_dribble_active();
    bool dribble_on(const std::string &filename);
    bool dribble_off();
    long int mem_used();
/* <-- debug }}}*/

/*{{{ run */
    long int run(long int runlimit = -1L);
    Values evaluate(const std::string &expression);
    Values function(const std::string &function_name, const std::string &arguments=std::string());
    void refresh_agenda();
    void refresh_agenda(const Module &module);
    void refresh_agenda(Module::pointer module);
    void reorder_agenda();
    void reorder_agenda(const Module &module);
    void reorder_agenda(Module::pointer module);
/*}}}*/

/*{{{ module --> */
    Module::pointer get_module(const std::string &module_name);
    Module::pointer get_module_list_head();
    std::vector<std::string> get_module_names();

    Module::pointer get_current_module();
    Module::pointer get_focused_module();
    std::vector<std::string> get_focus_stack();
/* <-- module }}}*/

/*{{{ global --> */
    Global::pointer get_global(const std::string &global_name);
    Global::pointer get_global_list_head();
    std::vector<std::string> get_globals_names();
    std::vector<std::string> get_globals_names(const Module &module);
    std::vector<std::string> get_globals_names(Module::pointer module);
/* <-- global }}}*/

/*{{{ function --> */
    Function::pointer get_function(const std::string &function_name);
    Function::pointer get_function_list_head();
    std::vector<std::string> get_function_names();
    std::vector<std::string> get_function_names(const Module &module);
    std::vector<std::string> get_function_names(Module::pointer module);
/* <-- function }}}*/

/*{{{ template --> */
    Template::pointer get_template(const std::string &template_name);
    Template::pointer get_template_list_head();
    std::vector<std::string> get_template_names();
    std::vector<std::string> get_template_names(const Module &module);
    std::vector<std::string> get_template_names(Module::pointer module);
/* <-- template }}}*/

/*{{{ facts --> */
    Fact::pointer get_facts();
    Fact::pointer assert_fact(const std::string &factString);

    DefaultFacts::pointer get_default_facts(const std::string &default_facts_name);
    std::vector< std::string > get_default_facts_names();
    std::vector<std::string> get_default_facts_names(const Module &module);
    std::vector<std::string> get_default_facts_names(Module::pointer module);
    DefaultFacts::pointer get_default_facts_list_head();
/* <-- facts }}}*/

/*{{{ class ---> */
    Class::pointer get_class(const std::string &class_name);
    Class::pointer get_class_list_head();
    std::vector<std::string> get_class_names();
    std::vector<std::string> get_class_names(const Module &module);
    std::vector<std::string> get_class_names(Module::pointer module);
/* <-- class }}}*/

/*{{{ instance --> */
    long save_instances(const std::string &filename, int saveCode = VISIBLE_SAVE);
    long binary_save_instances(const std::string &filename, int saveCode = VISIBLE_SAVE);
    long load_instances(const std::string &filename);
    long binary_load_instances(const std::string &filename);
    Instance::pointer make_instance(const std::string &makeString);
    Instance::pointer get_instance_list_head();
/* <-- instance }}}*/

/*{{{ rule --> */
    Rule::pointer get_rule(const std::string &rule_name);
    Rule::pointer get_rule_list_head();
    std::vector< std::string > get_rule_names();
    std::vector<std::string> get_rule_names(const Module &module);
    std::vector<std::string> get_rule_names(Module::pointer module);
    void remove_rules();
/* <-- rule }}}*/

/*{{{ add_function --> */
    template <typename T_return>
    bool add_function(std::string name, std::shared_ptr<Functor<T_return>> call);

    template <typename T_return, typename T_arg1>
    bool add_function(std::string name, std::shared_ptr<Functor<T_return, T_arg1>> call);

    template <typename T_return, typename T_arg1, typename T_arg2>
    bool add_function(std::string name, std::shared_ptr<Functor<T_return, T_arg1, T_arg2>> call);

    template <typename T_return, typename T_arg1, typename T_arg2, typename T_arg3>
    bool add_function(std::string name, std::shared_ptr<Functor<T_return, T_arg1, T_arg2, T_arg3>> call);

    template <typename T_return, typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4>
    bool add_function(std::string name, std::shared_ptr<Functor<T_return, T_arg1, T_arg2, T_arg3, T_arg4>> call);

    bool remove_function(std::string name);
/* <-- add_function }}}*/

/*{{{ get_callback --> */
    int (*get_callback(std::shared_ptr<Functor<std::string>> call))(void*)
    { return (UserFunc_t) (void* (*) (void*)) callback_string; }

    template <typename T_arg1>
    int (*get_callback(std::shared_ptr<Functor<std::string, T_arg1>> call))(void*)
    { return (UserFunc_t) (void* (*) (void*)) callback_string<T_arg1>; }

    template <typename T_arg1, typename T_arg2>
    int (*get_callback(std::shared_ptr<Functor<std::string, T_arg1, T_arg2>> call))(void*)
    { return (UserFunc_t) (void* (*) (void*)) callback_string<T_arg1, T_arg2>; }

    template <typename T_arg1, typename T_arg2, typename T_arg3>
    int (*get_callback(std::shared_ptr<Functor<std::string, T_arg1, T_arg2, T_arg3>> call))(void*)
    { return (UserFunc_t) (void* (*) (void*)) callback_string<T_arg1, T_arg2, T_arg3>; }

    template <typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4>
    int (*get_callback(std::shared_ptr<Functor<std::string, T_arg1, T_arg2, T_arg3, T_arg4>> call))(void*)
    { return (UserFunc_t) (void* (*) (void*)) callback_string<T_arg1, T_arg2, T_arg3, T_arg4>; }

    int (*get_callback(std::shared_ptr<Functor<Values>> call))(void*)
    { return  (UserFunc_t) (void (*) (void*, void*)) callback_multifield; }

    template <typename T_arg1>
    int (*get_callback(std::shared_ptr<Functor<Values, T_arg1>> call))(void*)
    { return  (UserFunc_t) (void (*) (void*, void*)) callback_multifield<T_arg1>; }

    template <typename T_arg1, typename T_arg2>
    int (*get_callback(std::shared_ptr<Functor<Values>> call))(void*)
    { return  (UserFunc_t) (void (*) (void*, void*)) callback_multifield<T_arg1, T_arg2>; }

    template <typename T_arg1, typename T_arg2, typename T_arg3>
    int (*get_callback(std::shared_ptr<Functor<Values>> call))(void*)
    { return  (UserFunc_t) (void (*) (void*, void*)) callback_multifield<T_arg1, T_arg2, T_arg3>; }

    template <typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4>
    int (*get_callback(std::shared_ptr<Functor<Values>> call))(void*)
    { return  (UserFunc_t) (void (*) (void*, void*)) callback_multifield<T_arg1, T_arg2, T_arg3, T_arg4>; }

    template <typename T_return>
    int (*get_callback(std::shared_ptr<Functor<T_return>> call))(void*)
    { return  (UserFunc_t) (T_return (*) (void*)) callback; }

    template <typename T_return, typename T_arg1>
    int (*get_callback(std::shared_ptr<Functor<T_return, T_arg1>> call))(void*)
    { return  (UserFunc_t) (T_return (*) (void*)) callback<T_return, T_arg1>; }

    template <typename T_return, typename T_arg1, typename T_arg2>
    int (*get_callback(std::shared_ptr<Functor<T_return, T_arg1, T_arg2>> call))(void*)
    { return  (UserFunc_t) (T_return (*) (void*)) callback<T_return, T_arg1, T_arg2>; }

    template <typename T_return, typename T_arg1, typename T_arg2, typename T_arg3>
    int (*get_callback(std::shared_ptr<Functor<T_return, T_arg1, T_arg2, T_arg3>> call))(void*)
    { return  (UserFunc_t) (T_return (*) (void*)) callback<T_return, T_arg1, T_arg2, T_arg3>; }

    template <typename T_return, typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4>
    int (*get_callback(std::shared_ptr<Functor<T_return, T_arg1, T_arg2, T_arg3, T_arg4>> call))(void*)
    { return  (UserFunc_t) (T_return (*) (void*)) callback<T_return, T_arg1, T_arg2, T_arg3, T_arg4>; }
/* <-- get_callback }}}*/

/*{{{ callback --> */
    static void *callback_string(void *theEnv);

    template <typename T_arg1>
    static void *callback_string(void *theEnv);

    template <typename T_arg1, typename T_arg2>
    static void *callback_string(void *theEnv);

    template <typename T_arg1, typename T_arg2, typename T_arg3>
    static void *callback_string(void *theEnv);

    template <typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4>
    static void *callback_string(void *theEnv);

    static void callback_multifield(void *theEnv, void *rv);

    template <typename T_arg1>
    static void callback_multifield(void *theEnv, void *rv);

    template <typename T_arg1, typename T_arg2>
    static void callback_multifield(void *theEnv, void *rv);

    template <typename T_arg1, typename T_arg2, typename T_arg3>
    static void callback_multifield(void *theEnv, void *rv);

    template <typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4>
    static void callback_multifield(void *theEnv, void *rv);

    template <typename T_return>
    static T_return callback(void *theEnv);

    template <typename T_return, typename T_arg1>
    static T_return callback(void *theEnv);

    template <typename T_return, typename T_arg1, typename T_arg2>
    static T_return callback(void *theEnv);

    template <typename T_return, typename T_arg1, typename T_arg2, typename T_arg3>
    static T_return callback(void *theEnv);

    template <typename T_return, typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4>
    static T_return callback(void *theEnv);
/* <-- callback }}}*/

/*{{{ get_function_restriction */
    char *get_function_restriction(std::string &name);

    template <typename T_arg1>
    char *get_function_restriction(std::string &name);

    template <typename T_arg1, typename T_arg2>
    char *get_function_restriction(std::string &name);

    template <typename T_arg1, typename T_arg2, typename T_arg3>
    char *get_function_restriction(std::string &name);

    template <typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4>
    char *get_function_restriction(std::string &name);
/* <-- get_function_restriction }}}*/

protected:
    std::map<std::string, char *> m_func_restr;
    std::map<std::string, Any> m_functors;

    static int get_arg_count(void *env);
    static void* get_function_context(void *env);
    static void  set_return_values(void *env, void *rv, const Values &v);
    static void* add_symbol(void *env, const char *s);

private:
    static std::map<void*, Environment*> m_environment_map;
    EnvironmentCallback *mCallback;

}; /* class Environment */

/*{{{ get_function_restriction --> */
inline char *Environment::get_function_restriction(std::string &name)
{
    if (m_func_restr.find(name) != m_func_restr.end())
        free(m_func_restr[name]);
    char *restr = (char *)malloc(4);
    m_func_restr[name] = restr;
    snprintf(restr, 4, "00u");
    return restr;
}

template <typename T_arg1>
inline char *Environment::get_function_restriction(std::string &name)
{
    if (m_func_restr.find(name) != m_func_restr.end())
        free(m_func_restr[name]);
    char *restr = (char *)malloc(5);
    m_func_restr[name] = restr;
    snprintf(restr, 5, "11u%c", get_argument_code<T_arg1>());
    return restr;
}

template <typename T_arg1, typename T_arg2>
inline char *Environment::get_function_restriction(std::string &name)
{
    if (m_func_restr.find(name) != m_func_restr.end())
        free(m_func_restr[name]);
    char *restr = (char *)malloc(6);
    m_func_restr[name] = restr;
    snprintf(restr, 6, "22u%c%c", get_argument_code<T_arg1>(), get_argument_code<T_arg2>());
    return restr;
}

template <typename T_arg1, typename T_arg2, typename T_arg3>
inline char *Environment::get_function_restriction(std::string &name)
{
    if (m_func_restr.find(name) != m_func_restr.end())
        free(m_func_restr[name]);
    char *restr = (char *)malloc(7);
    m_func_restr[name] = restr;
    snprintf(restr, 7, "33u%c%c%c", get_argument_code<T_arg1>(), get_argument_code<T_arg2>(),
        get_argument_code<T_arg3>());
    return restr;
}

template <typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4>
inline char *Environment::get_function_restriction(std::string &name)
{
    if (m_func_restr.find(name) != m_func_restr.end())
        free(m_func_restr[name]);
    char *restr = (char *)malloc(8);
    m_func_restr[name] = restr;
    snprintf(restr, 8, "44u%c%c%c%c", get_argument_code<T_arg1>(), get_argument_code<T_arg2>(),
        get_argument_code<T_arg3>(), get_argument_code<T_arg4>());
    return restr;
}
/* <-- get_function_restriction }}}*/

/*{{{ callback --> */
inline void* Environment::callback_string(void *theEnv)
{
    void *cbptr = get_function_context(theEnv);
    if (cbptr) {
        if (get_arg_count(theEnv) != 0)
            throw std::logic_error("clipsmm/string: wrong # args on slot callback; expected 0");
        Functor<std::string> *call = (Functor<std::string>*)(cbptr);
        return add_symbol(theEnv, ((*call)()).c_str());
    }
    throw;
}

template <typename T_arg1>
inline void* Environment::callback_string(void *theEnv)
{
    void *cbptr = get_function_context(theEnv);
    if (cbptr) {
        if (get_arg_count(theEnv) != 1)
            throw std::logic_error("clipsmm/string: wrong # args on slot callback; expected 1");
        T_arg1 arg1;
        get_argument(theEnv, 1, arg1);
        Functor<std::string, T_arg1> *call = (Functor<std::string, T_arg1>*)(cbptr);
        return add_symbol(theEnv, ((*call)(arg1)).c_str());
    }
    throw;
}

template <typename T_arg1, typename T_arg2>
inline void* Environment::callback_string(void *theEnv)
{
    void *cbptr = get_function_context(theEnv);
    if (cbptr) {
        if (get_arg_count(theEnv) != 2)
            throw std::logic_error("clipsmm/string: wrong # args on slot callback; expected 2");
        T_arg1 arg1;
        T_arg2 arg2;
        get_argument(theEnv, 1, arg1);
        get_argument(theEnv, 2, arg2);
        Functor<std::string, T_arg1, T_arg2> *call = (Functor<std::string, T_arg1, T_arg2>*)(cbptr);
        return add_symbol(theEnv, ((*call)(arg1, arg2)).c_str());
    }
    throw;
}

template <typename T_arg1, typename T_arg2, typename T_arg3>
inline void* Environment::callback_string(void *theEnv)
{
    void *cbptr = get_function_context(theEnv);
    if (cbptr) {
        if (get_arg_count(theEnv) != 3)
            throw std::logic_error("clipsmm/string: wrong # args on slot callback; expected 3");
        T_arg1 arg1;
        T_arg2 arg2;
        T_arg3 arg3;
        get_argument(theEnv, 1, arg1);
        get_argument(theEnv, 2, arg2);
        get_argument(theEnv, 3, arg3);
        Functor<std::string, T_arg1, T_arg2, T_arg3> *call = (Functor<std::string, T_arg1, T_arg2, T_arg3>*)(cbptr);
        return add_symbol(theEnv, ((*call)(arg1, arg2, arg3)).c_str());
    }
    throw;
}

template <typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4>
inline void* Environment::callback_string(void *theEnv)
{
    void *cbptr = get_function_context(theEnv);
    if (cbptr) {
        if (get_arg_count(theEnv) != 4)
            throw std::logic_error("clipsmm/string: wrong # args on slot callback; expected 4");
        T_arg1 arg1;
        T_arg2 arg2;
        T_arg3 arg3;
        T_arg4 arg4;
        get_argument(theEnv, 1, arg1);
        get_argument(theEnv, 2, arg2);
        get_argument(theEnv, 3, arg3);
        get_argument(theEnv, 4, arg4);
        Functor<std::string, T_arg1, T_arg2, T_arg3, T_arg4> *call = (Functor<std::string, T_arg1, T_arg2, T_arg3, T_arg4>*)(cbptr);
        return add_symbol(theEnv, ((*call)(arg1, arg2, arg3, arg4)).c_str());
    }
    throw;
}

inline void Environment::callback_multifield(void *theEnv, void *rv)
{
    void *cbptr = get_function_context(theEnv);
    if (cbptr) {
        if (get_arg_count(theEnv) != 0)
            throw std::logic_error( "clipsmm/mf: wrong # args on functor callback; expected 0" );
        Functor<Values> *call = (Functor<Values>*)(cbptr);
        Values v = (*call)();
        set_return_values(theEnv, rv, v);
        return;
    }
    throw;
}

template <typename T_arg1>
inline void Environment::callback_multifield(void *theEnv, void *rv)
{
    void *cbptr = get_function_context(theEnv);
    if (cbptr) {
        if (get_arg_count(theEnv) != 1)
            throw std::logic_error("clipsmm/mf: wrong # args on functor callback; expected 1");
        T_arg1 arg1;
        get_argument(theEnv, 1, arg1);
        Functor<Values, T_arg1> *call = (Functor<Values, T_arg1>*)(cbptr);
        Values v = (*call)(arg1);
        set_return_values(theEnv, rv, v);
        return;
    }
    throw;
}

template <typename T_arg1, typename T_arg2>
inline void Environment::callback_multifield(void *theEnv, void *rv)
{
    void *cbptr = get_function_context(theEnv);
    if (cbptr) {
        if (get_arg_count(theEnv) != 2)
            throw std::logic_error("clipsmm/mf: wrong # args on functor callback; expected 2");
        T_arg1 arg1;
        T_arg2 arg2;
        get_argument(theEnv, 1, arg1);
        get_argument(theEnv, 2, arg2);
        Functor<Values, T_arg1, T_arg2> *call = (Functor<Values, T_arg1, T_arg2>*)(cbptr);
        Values v = (*call)(arg1, arg2);
        set_return_values(theEnv, rv, v);
        return;
    }
    throw;
}

template <typename T_arg1, typename T_arg2, typename T_arg3>
inline void Environment::callback_multifield(void *theEnv, void *rv)
{
    void *cbptr = get_function_context(theEnv);
    if (cbptr) {
        if (get_arg_count(theEnv) != 3)
            throw std::logic_error("clipsmm/mf: wrong # args on functor callback; expected 3");
        T_arg1 arg1;
        T_arg2 arg2;
        T_arg3 arg3;
        get_argument(theEnv, 1, arg1);
        get_argument(theEnv, 2, arg2);
        get_argument(theEnv, 3, arg3);
        Functor<Values, T_arg1, T_arg2, T_arg3> *call = (Functor<Values, T_arg1, T_arg2, T_arg3>*)(cbptr);
        Values v = (*call)(arg1, arg2, arg3);
        set_return_values(theEnv, rv, v);
        return;
    }
    throw;
}

template <typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4>
inline void Environment::callback_multifield(void *theEnv, void *rv)
{
    void *cbptr = get_function_context(theEnv);
    if (cbptr) {
        if (get_arg_count(theEnv) != 4)
            throw std::logic_error("clipsmm/mf: wrong # args on functor callback; expected 4");
        T_arg1 arg1;
        T_arg2 arg2;
        T_arg3 arg3;
        T_arg4 arg4;
        get_argument(theEnv, 1, arg1);
        get_argument(theEnv, 2, arg2);
        get_argument(theEnv, 3, arg3);
        get_argument(theEnv, 4, arg4);
        Functor<Values, T_arg1, T_arg2, T_arg3, T_arg4> *call = (Functor<Values, T_arg1, T_arg2, T_arg3, T_arg4>*)(cbptr);
        Values v = (*call)(arg1, arg2, arg3, arg4);
        set_return_values(theEnv, rv, v);
        return;
    }
    throw;
}

template <typename T_return>
inline T_return Environment::callback(void* theEnv)
{
    void *cbptr = get_function_context(theEnv);
    if (cbptr) {
        if (get_arg_count(theEnv) != 0)
            throw std::logic_error("clipsmm: wrong # args on slot callback; expected 1");
        Functor<T_return> *call = (Functor<T_return>*)(cbptr);
        return (*call)();
    }
    throw;
}

template <typename T_return, typename T_arg1>
inline T_return Environment::callback(void* theEnv)
{
    void *cbptr = get_function_context(theEnv);
    if (cbptr) {
        if (get_arg_count(theEnv) != 1)
            throw std::logic_error("clipsmm: wrong # args on slot callback; expected 1");
        T_arg1 arg1;
        get_argument(theEnv, 1, arg1);
        Functor<T_return, T_arg1> *call = (Functor<T_return, T_arg1>*)(cbptr);
        return (*call)(arg1);
    }
    throw;
}

template <typename T_return, typename T_arg1, typename T_arg2>
inline T_return Environment::callback(void* theEnv)
{
    void *cbptr = get_function_context(theEnv);
    if (cbptr) {
        if (get_arg_count(theEnv) != 2)
            throw std::logic_error("clipsmm: wrong # args on slot callback; expected 2");
        T_arg1 arg1;
        T_arg2 arg2;
        get_argument(theEnv, 1, arg1);
        get_argument(theEnv, 2, arg2);
        Functor<T_return, T_arg1, T_arg2> *call = (Functor<T_return, T_arg1, T_arg2>*)(cbptr);
        return (*call)(arg1, arg2);
    }
    throw;
}

template <typename T_return, typename T_arg1, typename T_arg2, typename T_arg3>
inline T_return Environment::callback(void* theEnv)
{
    void *cbptr = get_function_context(theEnv);
    if (cbptr) {
        if (get_arg_count(theEnv) != 3)
            throw std::logic_error("clipsmm: wrong # args on slot callback; expected 3");
        T_arg1 arg1;
        T_arg2 arg2;
        T_arg3 arg3;
        get_argument(theEnv, 1, arg1);
        get_argument(theEnv, 2, arg2);
        get_argument(theEnv, 3, arg3);
        Functor<T_return, T_arg1, T_arg2, T_arg3> *call = (Functor<T_return, T_arg1, T_arg2, T_arg3>*)(cbptr);
        return (*call)(arg1, arg2, arg3);
    }
    throw;
}

template <typename T_return, typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4>
inline T_return Environment::callback(void* theEnv)
{
    void *cbptr = get_function_context(theEnv);
    if (cbptr) {
        if (get_arg_count(theEnv) != 4)
            throw std::logic_error("clipsmm: wrong # args on slot callback; expected 4");
        T_arg1 arg1;
        T_arg2 arg2;
        T_arg3 arg3;
        T_arg4 arg4;
        get_argument(theEnv, 1, arg1);
        get_argument(theEnv, 2, arg2);
        get_argument(theEnv, 3, arg3);
        get_argument(theEnv, 4, arg4);
        Functor<T_return, T_arg1, T_arg2, T_arg3, T_arg4> *call = (Functor<T_return, T_arg1, T_arg2, T_arg3, T_arg4>*)(cbptr);
        return (*call)(arg1, arg2, arg3, arg4);
    }
    throw;
}
/* <-- callback }}}*/

/*{{{ add_function --> */
template <typename T_return>
inline bool Environment::add_function(std::string name, std::shared_ptr<Functor<T_return>> call)
{
    m_functors[name] = call;
    return (EnvDefineFunction2WithContext(m_cobj,
            name.c_str(),
            get_return_code<T_return>(),
            get_callback(call),
            name.c_str(),
            get_function_restriction(name),
            (void*)call.get()));
}

template <typename T_return, typename T_arg1>
inline bool Environment::add_function(std::string name, std::shared_ptr<Functor<T_return, T_arg1>> call)
{
    m_functors[name] = call;
    return (EnvDefineFunction2WithContext(m_cobj,
            name.c_str(),
            get_return_code<T_return>(),
            get_callback(call),
            name.c_str(),
            get_function_restriction<T_arg1>(name),
            (void*)call.get()));
}

template <typename T_return, typename T_arg1, typename T_arg2>
inline bool Environment::add_function(std::string name, std::shared_ptr<Functor<T_return, T_arg1, T_arg2>> call)
{
    m_functors[name] = call;
    return (EnvDefineFunction2WithContext(m_cobj,
            name.c_str(),
            get_return_code<T_return>(),
            get_callback(call),
            name.c_str(),
            get_function_restriction<T_arg1,T_arg2>(name),
            (void*)call.get()));
}

template < typename T_return, typename T_arg1, typename T_arg2, typename T_arg3 >
inline bool Environment::add_function(std::string name, std::shared_ptr<Functor<T_return, T_arg1, T_arg2, T_arg3>> call)
{
    m_functors[name] = call;
    return (EnvDefineFunction2WithContext(m_cobj,
            name.c_str(),
            get_return_code<T_return>(),
            get_callback(call),
            name.c_str(),
            get_function_restriction<T_arg1,T_arg2, T_arg3>(name),
            (void*)call.get()));
}

template < typename T_return, typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4 >
inline bool Environment::add_function(std::string name, std::shared_ptr<Functor<T_return, T_arg1, T_arg2, T_arg3, T_arg4>> call)
{
    m_functors[name] = call;
    return (EnvDefineFunction2WithContext(m_cobj,
            name.c_str(),
            get_return_code<T_return>(),
            get_callback(call),
            name.c_str(),
            get_function_restriction<T_arg1,T_arg2, T_arg3, T_arg4>(name),
            (void*)call.get()));
}
/* <-- add_function }}}*/

} /* namespace CLIPS */

#endif /* __Environment_H__ */
