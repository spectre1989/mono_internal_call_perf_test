//
//  main.cpp
//  MonoEmbedTest
//
//  Created by Joe Best on 01/05/2015.
//  Copyright (c) 2015 Joe Best. All rights reserved.
//

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/class.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>

struct Widget
{
    int number;
};

Widget* widgets;
uint32_t num_widgets;
MonoClassField* native_handle_field;
MonoDomain* domain;
MonoClass* widget_class;
MonoObject** managed_widgets;

int ManagedLibrary_Widget_get_Number_Internal(const Widget* widget)
{
    return widget->number;
}

int ManagedLibrary_Widget_get_Number2(MonoObject* this_ptr)
{
    Widget* widget;
    mono_field_get_value(this_ptr, native_handle_field, reinterpret_cast<void*>(&widget));
    return widget->number;
}

MonoArray* ManagedLibrary_Widget_GetWidgets()
{
    MonoArray* array = mono_array_new(domain, widget_class, num_widgets);
    
    for(uint32_t i = 0; i < num_widgets; ++i)
    {
        mono_array_set(array, MonoObject*, i, managed_widgets[i]);
    }
    
    return array;
}

int main(int argc, const char * argv[])
{
    mono_set_dirs("/Library/Frameworks/Mono.framework/Home/lib", "/Library/Frameworks/Mono.framework/Home/etc");
    
    mono_config_parse(nullptr);
    
    const char* managed_binary_path = "/path/to/ManagedLibrary.dll";
    
    domain = mono_jit_init(managed_binary_path);
    MonoAssembly* assembly = mono_domain_assembly_open(domain, managed_binary_path);
    MonoImage* image = mono_assembly_get_image(assembly);
    
    mono_add_internal_call("ManagedLibrary.Widget::get_Number_Internal", reinterpret_cast<void*>(ManagedLibrary_Widget_get_Number_Internal));
    mono_add_internal_call("ManagedLibrary.Widget::get_Number2", reinterpret_cast<void*>(ManagedLibrary_Widget_get_Number2));
    mono_add_internal_call("ManagedLibrary.Widget::GetWidgets", reinterpret_cast<void*>(ManagedLibrary_Widget_GetWidgets));
    
    widget_class = mono_class_from_name(image, "ManagedLibrary", "Widget");
    native_handle_field = mono_class_get_field_from_name(widget_class, "native_handle");
    
    num_widgets = 5;
    widgets = new Widget[num_widgets];
    managed_widgets = new MonoObject*[num_widgets];
    
    for(uint32_t i = 0; i < num_widgets; ++i)
    {
        widgets[i].number = i * 4;
        
        MonoObject* managed_widget = mono_object_new(domain, widget_class);
        mono_runtime_object_init(managed_widget);
        void* native_handle_value = &widgets[i];
        mono_field_set_value(managed_widget, native_handle_field, &native_handle_value);
        
        managed_widgets[i] = managed_widget;
    }
    
    MonoClass* main_class = mono_class_from_name(image, "ManagedLibrary", "Main");
    
    const bool include_namespace = true;
    MonoMethodDesc* entry_point_method_desc = mono_method_desc_new("ManagedLibrary.Main:EntryPoint()", include_namespace);
    MonoMethod* entry_point_method = mono_method_desc_search_in_class(entry_point_method_desc, main_class);
    mono_method_desc_free(entry_point_method_desc);
    
    mono_runtime_invoke(entry_point_method, nullptr, nullptr, nullptr);
    
    mono_jit_cleanup(domain);
    
    delete[] widgets;
    delete[] managed_widgets;
    
    return 0;
}
