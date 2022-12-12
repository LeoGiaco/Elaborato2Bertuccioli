<h1> Istruzioni per l'esecuzione: </h1>

<ul>
    <li>
        <h2>Linux/uso diretto di g++:</h2>
        <ul>
            <li>Installare il pacchetto "libassimp-dev" usando il comando "apt-get";</li>
            <li>Scaricare la cartella, inserirci dentro una cartella di nome "bin" e lanciare da terminale il comando "make && ./main.out".</li>
        </ul>
    </li>
    <li>
        <h2>Visual Studio:</li>
        <ul>
            <li>Creare un nuovo progetto vuoto in C++ e inserire la cartella scaricata nella cartella interna del progetto (quella allo stesso livello del file .sln);</li>
            <li>Installare tramite la "Console di gestione pacchetti Nuget" i pacchetti "nupengl.core" e "glm" (comando "install-package");</li>
            <li>Assicurarsi di avere installate le librerie per assimp (vedi Virtuale)</li>
            <li>Nelle impostazioni di progetto, includere la libreria <h6>"assimp-vc142-mt.lib"</h6> e impostare la versione di compilazione <h6>c++17</h6></li>
            <li>Aggiungere tutti i file .h, .hpp e .cpp al progetto dalla barra laterale "Esplora soluzioni", cliccando rispettivamente su "File di intestazione" per i file .h e "File di origine" per i file .cpp (tasto destro -> Aggiungi -> Elemento esistente);</li>
            <li>Aprire il file "main.cpp" dentro la cartella e modificare la macro "PROJECT_FOLDER" con il nome della cartella scaricata seguito da "/";</li>
            <li>Lanciare in esecuzione il progetto tramite Visual Studio.</li>
        </ul>
    </li>
</ul>

<h1> Comandi: </h1>

<ul>
    <li><h6>Click del tasto sinistro del mouse</h6> per selezionare oggetti di scena (cliccare più volte sul pianeta cambia selezione tra il terreno e l'oceano);</li>
    <li><h6>Movimento del mouse con tasto sinistro premuto</h6> o <h6>Pressione dei tasti freccia</h6> per ruotare la telecamera attorno al pianeta;</li>
</ul>
<h2>Comandi per gli oggetti selezionati:</h2>
<ul>
    <li><h6>Tasto 's'</h6> per cambiare tra shader interpolato e shader su frammenti;</li>
    <li><h6>Tasto 'l'</h6> per cambiare tra modello di illuminazione phong e modello di illuminazione blinn-phong;</li>
    <li><h6>Tasto 'b'</h6> per attivare/disattivare lo shader cartoon;</li>
    <li><h6>Tasto 'c'</h6> per attivare/disattivare lo shader cartoon;</li>
    <li><h6>Tasti '+' e '-'</h6> per aumentare/diminuire il numero di bande di luminosità per lo shader cartoon;</li>
</ul>