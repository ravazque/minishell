Para agregar una nueva rama del proyecto creada en la página de GitHub a tu proyecto local desde la consola, sigue estos pasos:

### 1. **Verifica las ramas remotas**
   Primero, asegúrate de que la nueva rama creada en GitHub aparezca en las ramas remotas, ejecutando el siguiente comando:
   ```bash
   git fetch
   ```

### 2. **Lista las ramas remotas**
   Puedes listar todas las ramas disponibles en el repositorio remoto con este comando:
   ```bash
   git branch -r
   ```

   Esto mostrará algo como:
   ```
   origin/main
   origin/max-branch
   ```

   Aquí, `origin/max-branch` es la rama remota que quieres agregar localmente.

### 3. **Crear una copia local de la rama remota**
   Para crear una rama local que esté vinculada con la rama remota (por ejemplo, `max-branch`), usa:
   ```bash
   git checkout -b max-branch origin/max-branch
   ```

   Esto hará lo siguiente:
   - Creará una rama local llamada `max-branch`.
   - Vinculará la rama local con la rama remota `origin/max-branch`.

> [!flag b]-
> > La flag `-b` del comando `git checkout` significa **"crear y cambiar a una nueva rama"** al mismo tiempo.
> > 
> > ### Explicación:
> > - Cuando usas `git checkout -b <nombre-de-la-rama>`, Git crea una nueva rama con el nombre especificado y automáticamente cambia a esa nueva rama.
> > - Es equivalente a ejecutar dos comandos por separado:
> >   1. `git branch <nombre-de-la-rama>` (crear la rama).
> >   2. `git checkout <nombre-de-la-rama>` (cambiar a la rama creada).
> > 
> > ### Ejemplo:
> > ```bash
> > git checkout -b nueva-rama
> > ```
> > Esto:
> > 3. Crea una nueva rama llamada `nueva-rama`.
> > 4. Cambia automáticamente a esa rama.
> > 
> > Este comando es especialmente útil para agilizar el proceso de creación y cambio de ramas con un solo paso.

### 4. **Confirma que estás en la nueva rama**
   Verifica que has cambiado correctamente a la nueva rama:
   ```bash
   git branch
   ```

   Esto mostrará una lista de las ramas locales, con un asterisco (`*`) al lado de la rama activa:
   ```
   * max-branch
     main
   ```

### 5. **Sincroniza los cambios (si es necesario)**
   Si necesitas asegurarte de que tienes los cambios más recientes de la rama remota, ejecuta:
   ```bash
   git pull
   ```


## Que hacer con una rama una vez hecho un merge.

1. **Actualizar la rama**: Si quieres seguir trabajando en la misma rama sin perder su historial, puedes actualizarla al estado de `main` con los siguientes comandos:
   ```bash
   git checkout tu-rama
   git fetch origin
   git reset --hard origin/main
   ```
   Esto hará que `tu-rama` quede idéntica a `main`. Ten en cuenta que si tienes cambios no commitados en `tu-rama`, se perderán.

2. **Eliminar y crear una nueva rama**: Si no necesitas conservar el historial de la rama anterior, puedes eliminarla y crear una nueva basada en `main`:
   ```bash
   git branch -D tu-rama
   git checkout -b tu-rama origin/main
   ```
   Esto te asegura que empiezas desde un estado completamente limpio.

La opción depende de si prefieres mantener el historial de la rama o empezar desde cero. Si los cambios en la rama antigua ya no importan, eliminarla y crear una nueva suele ser la opción más clara y organizada.

