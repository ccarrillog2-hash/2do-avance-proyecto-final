#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include <string>
#include <limits>
#include <cmath>
#include <stdexcept>
#include <iomanip>

using namespace std;

float ventasPorMes[12][31] = {0};
int ventasPorMesUnidades[12][31] = {0};

struct producto {
    int codigo;
    char nombre [30];
    int stock;
    float precio;
    int ventasAcumuladas;
    bool activo;
};

struct DetalleVenta {
    int codigoproducto;
    int cantidad;
    float precioUnitario;
    float subtotal;
};
struct venta{
    int codigoventa;
    int dia;
    int mes;
    int cantidaddetalle;

    DetalleVenta detalles[10];
    float subtotal;
    float iva;
    float descuento;
    float total;
};

const char* ARCHIVO_PRODUCTOS = "productos.dat";
const char* ARCHIVO_VENTAS = "ventas.dat";



void MostrarMenu();
void RegistrarProducto();
void ListarProductos();
void BuscarPorCodigo();
void BuscarPorNombre();
void ActualizarStock();
void ModificarPrecio ();
void EliminarProducto();
void CrearVenta();
void ProductosMenorStock();
void ProductosMasVendidos();
void VentasTotalesDelDia();
void VentasPorMes();
void OrdenarProductosPorPrecio ();
void OrdenarStock ();
void OrdenarVentasAcumuladas();
void ExportarReporteTXT();
void ReiniciarInventario();
void MostrarEstadisticasGenerales();
void ReporteVentasMatriz();


int main() {
    cout << "Nombre del estudiante: Cindy Julissa Carrillo Guarcas\n";
    MostrarMenu();
    return 0;
}
void MostrarMenu(){
        int opcion;

        do{
            try{
                cout << "\n== Sistema de gestion de ventas e inventario para tienda ==\n";
                cout << "1.  Registrar Producto\n";
                cout << "2.  Listar Productos\n";
                cout << "3.  Buscar Por Codigo\n";
                cout << "4.  Buscar Por Nombre\n";
                cout << "5.  Actualizar Stock\n";
                cout << "6.  Modificar Precio\n";
                cout << "7.  Eliminar Producto\n";
                cout << "8.  Crear Venta\n";
                cout << "9.  Productos Menor Stock\n";
                cout << "10. Productos Mas Vendidos\n";
                cout << "11. Ventas Totales Del Dia\n";
                cout << "12. Ventas Por Mes\n";
                cout << "13. Ordenar Productos Por Precio\n";
                cout << "14. Ordenar Stock\n";
                cout << "15. Ordenar Ventas Acumuladas\n";
                cout << "16. Exportar ReporteTXT\n";
                cout << "17. Reiniciar Inventario\n";
                cout << "18. Mostrar Estadisticas Generales\n";
                cout << "19. Reporte Venta por Matriz\n";
                cout << "20. Salir\n";
                cout << "\nSelecciona una Opcion: ";
                cin >> opcion;

                if (cin.fail())
                    throw runtime_error("Opcion invalida");


                switch (opcion){
                case 1:
                    RegistrarProducto();
                    break;
                case 2:
                    ListarProductos();
                    break;
                case 3:
                    BuscarPorCodigo();
                    break;
                case 4:
                    BuscarPorNombre();
                    break;
                case 5:
                    ActualizarStock();
                    break;
                case 6:
                    ModificarPrecio();
                    break;
                case 7:
                    EliminarProducto();
                    break;
                case 8:
                    CrearVenta();
                    break;
                case 9:
                    ProductosMenorStock();
                    break;
                case 10:
                    ProductosMasVendidos();
                    break;
                case 11:
                    VentasTotalesDelDia();
                    break;
                case 12:
                    VentasPorMes();
                    break;
                case 13:
                    OrdenarProductosPorPrecio();
                    break;
                case 14:
                    OrdenarStock();
                    break;
                case 15:
                    OrdenarVentasAcumuladas();
                    break;
                case 16:
                    ExportarReporteTXT();
                    break;
                case 17:
                    ReiniciarInventario();
                    break;
                case 18:
                    MostrarEstadisticasGenerales();
                    break;
                case 19:
                    ReporteVentasMatriz();
                    break;
                case 20:
                    cout << "Saliendo del programa...\n";
                    break;
                default:
                    cout << "Opcion invalida.\n";
                }
            }
            catch(const exception& e) {
                cout << "\nERROR: " << e.what() << endl;
                cin.clear();
                cin.ignore(1000, '\n');
            }
        } while(opcion != 20);
}
void RegistrarProducto(){
    try{
        producto p;
        bool valido;

        cout << "\n===Registro de Producto ===\n";
        cout << "Ingrese Codigo: ";
        cin >> p.codigo;

        if(cin.fail() || p.codigo <= 0)
            throw runtime_error("No se acepta codigo con signo negativo");

        cin.ignore();
        do{
            valido = true;

        cout << "Ingrese Nombre: ";
        cin.getline(p.nombre,30);

            if(strlen(p.nombre) == 0){
                valido = false;
            }else{
                for(int i = 0; p.nombre[i] != '\0'; i++){
                    if(!isalpha(p.nombre[i]) && p.nombre[i] != ' '){
                        valido = false;
                        break;
                    }
                }
            }
            if(!valido)
                cout << "Nombre invalido (solo letras).\n";
        } while(!valido);

        cout << "Ingrese Stock: ";
        cin >> p.stock;

        if(cin.fail() || p.stock < 0)
            throw runtime_error("Stock invalido, solo numeros positivos");

        cout << "Ingrese Precio: ";
        cin >> p.precio;

        if(cin.fail() || p.precio <= 0)
            throw runtime_error("Precio invalido (solo numeros)");

        p.ventasAcumuladas = 0;
        p.activo = true;

        ofstream archivo(ARCHIVO_PRODUCTOS, ios::binary | ios::app);
        archivo.write(reinterpret_cast<char*>(&p), sizeof(producto));
        archivo.close();

        cout << "\nProducto guardado correctamente.\n";
    }
    catch (const exception& e) {
        cout << "Erro al registrar producto: " << e.what() << endl;
    }
}
void ListarProductos(){
    try{
        producto p;

        ifstream archivo(ARCHIVO_PRODUCTOS, ios::binary);
        if(!archivo)
            throw runtime_error("No existe producto");

        cout << "\n=====Lista de Producto =====\n";

        while (archivo.read(reinterpret_cast<char*>(&p), sizeof(producto))){
            if (p.activo) {
                cout << "Codigo : " << p.codigo
                       << " | Nombre : " << p.nombre
                       << " | Stock : " << p.stock
                       << " | Precio : Q" << p.precio
                       << " | Ventas : " << p.ventasAcumuladas
                       << " | Activo : " << p.activo << "\n";
            }
        }
        archivo.close();
    }catch (const exception& e){
        cout << "Error de lista de producto: " << e.what() << endl;
    }
}
void BuscarPorCodigo(){
    try{
        producto p;
        int codigo;

        bool encontrado = false;
        bool valido = false;

        cout << "\n======Busqueda por codigo ======\n";
        while(!valido){
        cout << "Ingrese Codigo: ";
        cin >> codigo;

        if(cin.fail()){
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Error: solo se aceptan numeros. Intente nuevamente.\n";
        } else {
            cin.ignore(1000, '\n');
            valido = true;
        }
        }

        ifstream archivo(ARCHIVO_PRODUCTOS, ios::binary);
        while (archivo.read(reinterpret_cast<char*>(&p), sizeof(producto))){
            if (p.codigo == codigo && p.activo) {
                cout << "\nProducto Encontrado\n";
                cout << "Codigo : " << p.codigo << "\n";
                cout << "Nombre: " << p.nombre << endl;
                cout << "Stock: " << p.stock << endl;
                cout << "Precio: Q" << p.precio << endl;
                encontrado = true;
                break;
            }
        }
        archivo.close();

        if (!encontrado){
            cout << "No existe codigo. Intente nuevamente.\n";
        }
    }
    catch (const exception& e){
        cout << "Error de codigo: " << e.what() << endl;
    }
}
void BuscarPorNombre(){
    try {
        producto p;
        char nombre [30];
        bool encontrado = false;
        bool valido = false;


        cout << "\n=== Busqueda por nombre ===\n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        while(!valido){
        cout << "Ingrese Nombre: ";
        cin.getline(nombre,30);

            valido = true;

            for(int i = 0; nombre[i] != '\0'; i++){
                if(!isalpha(nombre[i]) && nombre[i] != ' '){
                    valido = false;
                    break;
                }
            }
            if(!valido){
                cout << "Error: solo se aceptan letras. Intente nuevamente.\n";
            }
        }
        ifstream archivo(ARCHIVO_PRODUCTOS, ios::binary);
        if (!archivo){
            cout << "No se pudo abrir el archivo de productos\n";
            return;
        }

        while (archivo.read(reinterpret_cast<char*>(&p), sizeof(producto))) {
            if (p.activo && strcmp(p.nombre, nombre) == 0) {
                cout << "\nSe encontro el producto\n";
                cout << "Codigo: " << p.codigo << endl;
                cout << "Nombre: " << p.nombre << endl;
                cout << "Stock: " << p.stock << endl;
                cout << "Precio: Q" << p.precio << endl;
                encontrado = true;
            }
        }
        archivo.close();
        if (!encontrado){
            cout << "No existe producto\n";
        }
    }
    catch(const exception& e){
        cout << "Error busqueda de nombre: " << e.what() << endl;
    }
}
        void ActualizarStock(){
    try {
        producto p;
        int codigo, stock;
        bool encontrado = false;

        cout << "\n===Actualizar Stock ===\n";
        cout << "Ingrese codigo del producto actual: ";
        cin >> codigo;

        if (cin.fail())
            throw runtime_error("Codigo invalido");

        cout << "Ingrese Nuevo Stock: ";
        cin >> stock;

        if(cin.fail() || stock < 0)
            throw runtime_error("Stock invalido");

        fstream archivo(ARCHIVO_PRODUCTOS, ios::binary | ios::in | ios::out);

        while (archivo.read(reinterpret_cast<char*>(&p), sizeof(producto))){
            if (p.codigo == codigo && p.activo) {
                p.stock = stock;

                archivo.seekp(-static_cast<int>(sizeof(producto)), ios::cur);
                archivo.write(reinterpret_cast<char*>(&p), sizeof(producto));
                encontrado = true;

                cout << "Stock actualizado correctamente.\n";
                break;
            }
        }
        archivo.close();
        if (!encontrado){
            cout << "No hay stock registrado.\n";
        }
    }catch(const exception& e){
        cout << "Error stock: " << e.what() << endl;
    }
}
        void ModificarPrecio(){
    try{
        producto p;
        int codigo;
        float precio;
        bool encontrado = false;

        cout << "\n====Modificar Precio====\n";
        cout << "Ingreser codigo del producto:";
        cin >> codigo;

        cout << "Nuevo Precio: Q ";
        cin >> precio;

        if(cin.fail() || precio <= 0)
            throw runtime_error("Precio invalido");

        fstream archivo(ARCHIVO_PRODUCTOS,ios::binary | ios::in | ios::out);

        while (archivo.read(reinterpret_cast<char*>(&p), sizeof(producto))){
            if (p.codigo == codigo && p.activo) {
                p.precio = precio;

                archivo.seekp(-static_cast<int>(sizeof(producto)), ios::cur);
                archivo.write(reinterpret_cast<char*>(&p), sizeof(producto));
                encontrado = true;

                cout << "Precio actualizado correctamente.\n";
                break;
            }
        }
        archivo.close();

        if (!encontrado){
            cout << "Producto no encontrado.\n";
        }
    }catch(const exception& e){
        cout << "Error precio: " << e.what() << endl;
    }
}
        void EliminarProducto(){
    try {
        producto p;
        int codigo;
        bool encontrado = false;

        cout << "\n===Eliminar Producto===\n";
        cout << "Ingrese codigo del producto actual: ";
        cin >> codigo;

        if (cin.fail())
            throw runtime_error("Codigo invalido");

        fstream archivo(ARCHIVO_PRODUCTOS, ios::in | ios::out | ios::binary);
        if (!archivo.is_open())
            throw runtime_error("No hay codigo de producto registrados...");

        while (archivo.read(reinterpret_cast<char*>(&p), sizeof(producto))){
            if(p.codigo == codigo && p.activo){
                p.activo = false;
                archivo.seekp(-static_cast<int>(sizeof(producto)), ios::cur);
                archivo.write(reinterpret_cast<char*>(&p), sizeof(producto));
                encontrado = true;

                cout << "Producto eliminado exitosamente.\n";
                break;
            }
        }
        archivo.close();
        if (!encontrado){
            cout << "Codigo de producto no existe.\n";
        }
    } catch(const exception& e){
        cout << "Error eliminar: " << e.what() << endl;
    }
}
    void CrearVenta(){
    try{
        producto p;
        venta nuevaventa = {};

        int codigoproducto;
        int cantidad;
        int totalarticulo = 0;

        nuevaventa.subtotal = 0;
        nuevaventa.iva = 0;
        nuevaventa.descuento = 0;

        cout << "\n=== Crear Venta ===\n";

        cout << "Codigo de venta: ";
        cin >> nuevaventa.codigoventa;

        cout << "Dia (1-31): ";
        cin >> nuevaventa.dia;
        if(nuevaventa.dia < 1 || nuevaventa.dia > 31)
            throw runtime_error("Dia invalido");

        cout << "Mes (1-12): ";
        cin >> nuevaventa.mes;
        if(nuevaventa.mes < 1 || nuevaventa.mes > 12)
            throw runtime_error("Mes invalido");

        cout << "Cantidad de productos (max 10): ";
        cin >> nuevaventa.cantidaddetalle;

        if(cin.fail() || nuevaventa.cantidaddetalle <= 0 || nuevaventa.cantidaddetalle > 10)
            throw runtime_error("Cantidad de productos invalida");

        for (int i = 0; i < nuevaventa.cantidaddetalle; i++){

            cout << "\n=== Producto " << i + 1 << " ===\n";
            cout << "Codigo producto: ";
            cin >> codigoproducto;

            cout << "Cantidad: ";
            cin >> cantidad;

            if(cin.fail() || cantidad <= 0)
                throw runtime_error("Cantidad invalida");

            bool encontrado = false;

            fstream archivo(ARCHIVO_PRODUCTOS, ios::in | ios::out | ios::binary);

            while (archivo.read(reinterpret_cast<char*>(&p), sizeof(producto))){

                if(p.codigo == codigoproducto && p.activo){
                    encontrado = true;

                    if(cantidad > p.stock){
                        throw runtime_error("Stock insuficiente para el producto");
                    }

                    nuevaventa.detalles[i].codigoproducto = codigoproducto;
                    nuevaventa.detalles[i].cantidad = cantidad;
                    nuevaventa.detalles[i].precioUnitario = p.precio;
                    nuevaventa.detalles[i].subtotal = p.precio * cantidad;

                    nuevaventa.subtotal += nuevaventa.detalles[i].subtotal;
                    totalarticulo += cantidad;

                    // Stock y ventas
                    p.stock -= cantidad;
                    p.ventasAcumuladas += cantidad;

                    ventasPorMes[nuevaventa.mes - 1][0] += nuevaventa.detalles[i].subtotal;

                    archivo.seekp(-(long)sizeof(producto), ios::cur);
                    archivo.write(reinterpret_cast<char*>(&p), sizeof(producto));
                    break;
                }
            }

            archivo.close();

            if(!encontrado){
                cout << "Producto no encontrado, intente de nuevo.\n";
                i--;
            }
        }
        nuevaventa.iva = nuevaventa.subtotal * 0.12;
        nuevaventa.descuento = 0;

        cout << "\n===Aplicando descuentos ===\n";

        if(nuevaventa.subtotal > 500){
            float desc = nuevaventa.subtotal * 0.05;
            nuevaventa.descuento += desc;
            cout << "Descuento 5% por subtotal > 500: Q" << desc << endl;
        }
        else{
            cout << "No aplica descuento por subtotal\n";
        }

        if(totalarticulo % 5 == 0 && totalarticulo > 0){
            float desc = nuevaventa.subtotal * 0.02;
            nuevaventa.descuento += desc;
            cout << "Descuento 2% por cantidad de articulos multiple de 5: Q" << desc << endl;
        }
        else{
            cout << "No aplica descuento por cantidad de articulos\n";
        }

        nuevaventa.total = nuevaventa.subtotal + nuevaventa.iva - nuevaventa.descuento;
        nuevaventa.total = round(nuevaventa.total * 100) / 100;

        ofstream archivoventa(ARCHIVO_VENTAS, ios::binary | ios::app);
        if(!archivoventa){
            throw runtime_error("No se pudo guardar la venta");
        }
        archivoventa.write(reinterpret_cast<char*>(&nuevaventa), sizeof(venta));
        archivoventa.close();

        cout << "=================================\n";
        cout << "Venta creada y guarda exitosamente\n";
        cout << "=================================\n";

        cout << fixed << setprecision(2);
        cout << "\n=== Venta registrada correctamente ===\n";
        cout << "Venta No: " << nuevaventa.codigoventa << endl;
        cout << "Subtotal: Q" << nuevaventa.subtotal << endl;
        cout << "IVA: Q" << nuevaventa.iva << endl;
        cout << "Descuento: Q" << nuevaventa.descuento << endl;
        cout << "Total: Q" << nuevaventa.total << endl;
    }
    catch(const exception& e){
        cout << "\nERROR EN VENTA: " << e.what() << endl;
    }
}
            void ProductosMenorStock(){
            try {
                producto p;

        ifstream archivo(ARCHIVO_PRODUCTOS, ios::binary);
                if(!archivo){
                    cout << "No hay productos.\n";
                    return;
                }
        cout << "\n---Productos con menos stock ---\n";
        while (archivo.read(reinterpret_cast<char*>(&p), sizeof(producto)))
        {
            if(p.activo && p.stock <= 5)
            {
                cout << "\nCodigo: " << p.codigo;
                cout << "\nNombre: " << p.nombre;
                cout << "\nStock: " << p.stock;
            }
        }
        archivo.close();
    }
    catch(const exception& e){
        cout << "Error menor stock: " << e.what() << endl;
    }
}
        void ProductosMasVendidos(){
        try{
        producto p;

        ifstream archivo(ARCHIVO_PRODUCTOS, ios::binary);
            if(!archivo){
                cout << "No hay productos registrados.\n";
                return;
            }

        while (archivo.read(reinterpret_cast<char*>(&p), sizeof(producto))){
            if (p.activo && p.ventasAcumuladas > 0){
                cout << "\n====Productos Mas Vendidos ====\n";
                cout << "\nCodigo: " << p.codigo;
                cout << "\nNombre: " << p.nombre;
                cout << "\nVentas: " << p.ventasAcumuladas;
            }
        }
        archivo.close();
    }
        catch(const exception& e){
        cout << "ERROR mas vendidos: " << e.what() << endl;
    }
}

void VentasTotalesDelDia(){
    try{
        producto p;
        venta nuevaventa;
        int dia;
        float total=0;
        bool encontrado = false;

        ifstream archivo(ARCHIVO_VENTAS, ios::binary);
        if(!archivo){
            cout << "No hay ventas registradas.\n";
            return;
        }
        cout << "\n====Venta por dia====\n";
        cout << "Ingrese dia: ";
        cin >> dia;

        while (archivo.read(reinterpret_cast<char*>(&nuevaventa), sizeof(venta))){
            if(nuevaventa.dia == dia){
                total += nuevaventa.total;
                encontrado = true;
            }
        }
        archivo.close();

        if(encontrado){
            cout << "Total del dia: Q" << total << endl;
        } else {
            cout << "No hay ventas registradas para ese dia.\n";
        }
    } catch(const exception& e){
        cout << "Error en ventas del dia: " << e.what() << endl;
    }
}

void VentasPorMes(){
    try{
        producto p;
        venta nuevaventa;
        int mes;
        float total=0;
        bool encontrado = false;

        cout << "\n==== Venta por mes ====\n";
        cout << "Ingrese mes: ";
        cin >> mes;

        if(mes < 1 || mes > 12)
            throw runtime_error("Mes invalido");

        ifstream archivo(ARCHIVO_VENTAS, ios::binary);
        if(!archivo){
            cout << "No hay ventas registradas.\n";
            return;
        }
        while (archivo.read(reinterpret_cast<char*>(&nuevaventa), sizeof(venta))){
            if(nuevaventa.mes == mes){
                total += nuevaventa.total;
                encontrado = true;
            }
        }
            archivo.close();
    if(encontrado){
        cout << "Total del mes: Q" << total << endl;
    } else
        {
        cout << "No hay ventas registradas para ese mes.\n";
    }

    }
    catch(const exception& e){
        cout << "Error en ventas por mes: " << e.what() << endl;
    }
}

void OrdenarProductosPorPrecio(){
    try{
        producto p[100];
        int n = 0;
        int opcion;

        ifstream archivo(ARCHIVO_PRODUCTOS, ios::binary);
        if(!archivo){
            cout << "No hay productos.\n";
            return;
        }
        while (archivo.read(reinterpret_cast<char*>(&p[n]), sizeof(producto))){
            if(p[n].activo){
                n++;
            }
        }
            archivo.close();

            cout << "\n1. Ascendente\n";
            cout << "2. Descendente\n";
            cout << "Opcion: ";
            cin >> opcion;

            if(cin.fail()){
                cout << "Opcion invalida\n";
                return;
            }
            for(int i = 0; i < n - 1; i++){
                for(int registroProducto = 0; registroProducto < n - i - 1; registroProducto++){
                    bool condicion;

                    if(opcion == 1){
                        condicion = p[registroProducto].precio > p[registroProducto + 1].precio;
                    }
                    else{
                        condicion = p[registroProducto].precio < p[registroProducto + 1].precio;
                    }

                    if(condicion){
                        producto temp = p[registroProducto];
                        p[registroProducto] = p[registroProducto + 1];
                        p[registroProducto + 1] = temp;
                    }
                }
            }
            cout << "\n--- PRODUCTOS ORDENADOS POR PRECIO ---\n";
            for(int i = 0; i < n; i++){
                cout << p[i].codigo << " - "
                     << p[i].nombre << " - Q"
                     << p[i].precio << endl;
            }
        }
        catch(const exception& e){
            cout << "Error inesperado en ordenamiento: " << e.what() << endl;
    }
}

void OrdenarStock(){
    try
    {
        producto p[100];
        int n = 0;
        int opcion;

        ifstream archivo(ARCHIVO_PRODUCTOS, ios::binary);
        if(!archivo){
            cout << "No existe productos.\n";
            return;
        }

        while (archivo.read(reinterpret_cast<char*>(&p[n]), sizeof(producto))){
            if(p[n].activo){
                n++;
            }
        }

        archivo.close();

        cout << "\n1. Ascendente\n2. Descendente\nOpcion: ";
        cin >> opcion;

        if(cin.fail()){
            cout << "Opcion invalida\n";
            return;
        }

        for(int i = 0; i < n - 1; i++){
            for(int registroProducto = 0; registroProducto < n - i - 1; registroProducto++){
                bool condicion;

                if(opcion == 1)
                    condicion = p[registroProducto].stock > p[registroProducto + 1].stock;
                else
                    condicion = p[registroProducto].stock < p[registroProducto + 1].stock;

                if(condicion){
                    producto temp = p[registroProducto];
                    p[registroProducto] = p[registroProducto+1];
                    p[registroProducto+1] = temp;
                }
            }
        }

        cout << "\n--- Productos ordenados por stock---\n";
        for(int i = 0; i < n; i++){
            cout << p[i].codigo << " - "
                 << p[i].nombre << " - Stock: "
                 << p[i].stock << endl;
        }
    }
    catch(const exception& e){
        cout << "Error: " << e.what() << endl;
    }
}
        void OrdenarVentasAcumuladas(){
            try{
                producto p[100];
                int n = 0;
                int opcion;

                ifstream archivo(ARCHIVO_PRODUCTOS, ios::binary);
                if(!archivo){
                    cout << "No hay productos.\n";
                    return;
                }
                while (archivo.read(reinterpret_cast<char*>(&p[n]), sizeof(producto))){
                    if(p[n].activo){
                        n++;
                        if (n >= 100) break;
                    }
                }
                archivo.close();

                cout << "\n1. Ascendente\n";
                cout << "2. Descendente\n";
                cout << "Opcion: ";
                cin >> opcion;

                if(cin.fail()){
                    cout << "Opcion invalida\n";
                    return;
                }

                for(int i = 0; i < n - 1; i++){
                    for(int registroProducto = 0; registroProducto < n - i - 1; registroProducto++){
                        bool condicion;

                        if(opcion == 1)
                            condicion = p[registroProducto].ventasAcumuladas > p[registroProducto+1].ventasAcumuladas;
                        else
                            condicion = p[registroProducto].ventasAcumuladas < p[registroProducto+1].ventasAcumuladas;

                        if(condicion){
                            producto temp = p[registroProducto];
                            p[registroProducto] = p[registroProducto+1];
                            p[registroProducto+1] = temp;
                        }
                    }
                }

                cout << "\n--- Produdcto ordenados por ventas ---\n";
                for(int i = 0; i < n; i++){
                    cout << p[i].codigo << " - " << p[i].nombre
                         << " - Ventas: " << p[i].ventasAcumuladas << endl;
                }
            }
    catch(const exception& e){
                cout << "Error: " << e.what() << endl;
            }
}

void ExportarReporteTXT(){
    try{
        producto p;

        ofstream reporte("reporte.txt");
        if(!reporte){
            cout << "No se pudo crear el archivo.\n";
            return;
        }
        ifstream archivo(ARCHIVO_PRODUCTOS, ios::binary);
                while (archivo.read(reinterpret_cast<char*>(&p), sizeof(producto))){
            if(p.activo){
                reporte << p.codigo << " | "
                        << p.nombre << " | "
                        << p.stock << " | "
                        << p.precio << " | "
                        << p.ventasAcumuladas << "\n";
            }
        }
        archivo.close();
        reporte.close();

        cout << "Reporte exportado exitosamente.\n";
    }
    catch(const exception& e){
        cout << "Error al exportar reporte: " << e.what() << endl;
    }
}

void ReiniciarInventario(){
    try{
        char confirmacion;
        cout << "Seguro que deseas reiniciar inventario (s/n): ";
        cin >> confirmacion;

        if(confirmacion != 's' && confirmacion != 'S'){
            cout << "Operacion cancelada.\n";
            return;
        }

        ofstream archivo(ARCHIVO_PRODUCTOS, ios::binary | ios::trunc);
        archivo.close();

        cout << "Inventario reiniciado correctamente.\n";

    }
    catch(const exception& e){
        cout << "Error: " << e.what() << endl;
    }
}

void MostrarEstadisticasGenerales(){
    try{
        producto p;

        int totalproducto = 0;
        int totalstock = 0;
        float valorinventario = 0;
        int totalventas = 0;

        ifstream archivo(ARCHIVO_PRODUCTOS, ios::binary);
        if(!archivo)
            throw runtime_error("No se encuentra archivo");

            while (archivo.read(reinterpret_cast<char*>(&p), sizeof(producto))){
                if (p.activo){
                    totalproducto++;
                    totalstock += p.stock;
                    valorinventario += p.stock * p.precio;
                    totalventas += p.ventasAcumuladas;
                }
        }
        archivo.close();
        cout << "\n--- Mostrar Estadistica General ---\n";
        cout << "Productos activos: " << totalproducto << endl;
        cout << "Total Stock: " << totalstock << endl;
        cout << "Ventas acumuladas: " << totalventas << endl;
        cout << "Valor inventario: Q" << valorinventario << endl;
    }
    catch(const exception& e){
        cout << "ERROR estadisticas: " << e.what() << endl;
    }
}
void ReporteVentasMatriz(){
    try{
    cout << "\n--- Ventas por mes (matriz) ---\n";

    for(int mes = 0; mes < 12; mes++){
        float totalMes = 0;

        for(int dia = 0; dia < 31; dia++){
            totalMes += ventasPorMes[mes][dia];
        }

        cout << "Mes " << mes + 1 << ": Q" << totalMes << endl;
    }
}
    catch(const exception& e){
        cout << "Error reporte matriz: " << e.what() << endl;
    }
}