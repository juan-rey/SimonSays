# Guia de ajuda do SimonSays

## Visão geral
O SimonSays é um assistente de conversão de texto em voz (TTS) para Windows criado por Juan Rey Saura. Nasceu como uma ferramenta para pessoas com ELA e outras pessoas que já não conseguem falar, para que pudessem comunicar rapidamente sem largar o que estavam a fazer no computador. Com o tempo tornou-se claro que o mesmo conceito serve como ferramenta geral de CAA (comunicação aumentativa e alternativa) para qualquer pessoa para quem falar é difícil ou impossível; por exemplo, devido a disartria, laringectomia, apraxia da fala, um AVC ou uma lesão cerebral. Está otimizado para falar depressa, usar bibliotecas de frases e reproduzir sons no meio do texto. Suporta vários idiomas, vozes configuráveis e funcionalidades pensadas para uma comunicação eficiente.

O SimonSays usa o espaço livre da barra de tarefas do Windows e dá acesso rápido a uma dúzia de categorias de frases frequentes (e personalizáveis) com apenas um par de cliques. Como nunca tapa as outras janelas, pode continuar no navegador, nos seus documentos ou numa videochamada e falar sempre que precisar: com o teclado, com o rato ou sem mãos, com um dispositivo de seguimento ocular.

Casos de utilização:
- Utilização intensiva de várias aplicações do Windows sem perder de vista a comunicação.
- Quadros criados por terapeutas da fala para pessoas com perturbações da fala.
- Reuniões online com o [Virtual CABLE](https://vb-audio.com/Cable/) (consulte [Usar o SimonSays em reuniões online](#usar-o-simonsays-em-reuniões-online)).

O SimonSays não pretende substituir programas de comunicação completos como o Mind Express, o TD Communicator, o Grid 3 ou o OptiKey, nem a avaliação e o apoio profissional que costumam acompanhá-los. Faz um trabalho mais específico: falar rapidamente a partir da barra de tarefas enquanto continua a trabalhar noutras aplicações. Pode funcionar ao lado desses programas; por exemplo, o OptiKey ou outro software de controlo ocular pode mover o cursor enquanto o SimonSays diz as suas frases.

O SimonSays é de código aberto e gratuito (livre como em liberdade, não só como em preço), e continuará a sê-lo: qualquer pessoa o pode usar, estudar, adaptar e partilhar. Não tem publicidade e não recolhe dados.

O SimonSays está perto da versão 1.0. Antes dessa meta, precisa de pessoas que o experimentem no dia a dia, com as suas próprias vozes, quadros e dispositivos de seguimento ocular: se algo falhar, for incómodo ou faltar, diga-nos (consulte [Comentários, suporte e privacidade](#comentários-suporte-e-privacidade)). Cada comentário ajuda.

## Índice
- [Primeiros passos](#primeiros-passos)
- [Janela de categorias](#janela-de-categorias)
- [Frases predefinidas](#frases-predefinidas)
- [Atalhos](#atalhos)
- [Definições (F2)](#definições-f2)
- [Olhar / Clique por fixação (F3)](#olhar--clique-por-fixação-f3)
- [Usar o SimonSays em reuniões online](#usar-o-simonsays-em-reuniões-online)
- [Menu do ícone da área de notificação](#menu-do-ícone-da-área-de-notificação)
- Editar quadros (avançado):
  - [Editar categorias e frases](#editar-categorias-e-frases) (e [que editor usar](#que-editor-usar))
  - [Misturar voz e sons](#misturar-voz-e-sons)
  - [Personalizar o aspeto (estilos do quadro e das categorias)](#personalizar-o-aspeto-estilos-do-quadro-e-das-categorias)
  - [Importar e exportar categorias](#importar-e-exportar-categorias)
  - [Anular alterações](#anular-alterações)
  - [Onde o SimonSays guarda os seus ficheiros](#onde-o-simonsays-guarda-os-seus-ficheiros)
- [Resolução de problemas](#resolução-de-problemas)
- [Comentários, suporte e privacidade](#comentários-suporte-e-privacidade)

## Primeiros passos
1. Inicie o SimonSays. Aparece num espaço livre da barra de tarefas do Windows e acrescenta um ícone à área de notificação. A barra de tarefas pode estar em baixo, em cima, à esquerda ou à direita do ecrã:
   - Quando os ícones da barra de tarefas estão centrados, o SimonSays fica mesmo no início da barra (na ponta esquerda, ou em cima numa barra lateral). Quando estão alinhados à esquerda, fica logo antes da área de notificação.
   - Numa barra de tarefas pequena (estreita), a barra do SimonSays fica mais baixa, com letra mais pequena e os dois botões de acesso rápido lado a lado. Numa barra de tarefas à esquerda ou à direita do ecrã, os controlos ficam empilhados uns por baixo dos outros.
   - A janela de categorias abre-se junto à barra de tarefas, e a seta do botão `Categorias` indica para que lado se abre.
   - Se mover ou redimensionar a barra de tarefas enquanto o SimonSays está em execução, ele acompanha-a em poucos segundos.
2. Para falar, pode:
   - Clicar na caixa de texto da barra (ou fixar o olhar nela): abre-se uma caixa de introdução maior junto à barra de tarefas. Escreva o texto e prima `Enter` para o dizer; a caixa fecha-se e o foco passa para `Reproduzir>`, pelo que `Espaço` volta a dizê-lo (ou para-o). `Esc` fecha a caixa sem falar e leva-o de volta à janela que estava a usar; o texto fica na barra, onde `Reproduzir>` o volta a dizer.
   - Clicar numa ou em várias frases guardadas na janela de categorias.
3. Durante a reprodução, o botão mostra `>` para indicar que está a tocar. Pode premi-lo de novo para parar de imediato.

![A barra do SimonSays na barra de tarefas](img/bar.png)

Junto ao botão `Categorias`, dois pequenos **botões de acesso rápido** dão acesso com um clique a duas coisas a que, de outro modo, chegaria com um atalho ou com o menu da área de notificação:

- 📂 abre um quadro guardado no seu computador; é a mesma importação que `F9`, mas começa na sua pasta de quadros (`%LocalAppData%\SimonSays\boards\`), onde ficam os quadros exportados e transferidos (consulte [Importar e exportar categorias](#importar-e-exportar-categorias)).
- ⚙️ abre as definições, tal como `F2`.

Ambos podem ser ocultados com `Mostrar botões e menu de acesso rápido` nas definições (`F2`), o que deixa mais espaço para a caixa de introdução. A mesma opção desativa também o menu de contexto (clique com o botão direito) da janela de categorias.

Se o SimonSays estiver oculto, clique no seu ícone na área de notificação para o mostrar de novo.

## Janela de categorias
![A janela de categorias com as frases predefinidas](img/categories.png)

- Abra-a com o botão `Categorias`. Redimensione-a, ou ative `Lembrar o tamanho da janela de categorias` nas definições para manter o tamanho.
- Selecione uma categoria na lista de cima e frases na lista de baixo.
- Um clique numa frase di-la automaticamente se `Falar a frase imediatamente ao selecionar` estiver ativo; caso contrário, a frase é carregada na caixa de introdução principal sem ser dita.
- Faça duplo clique numa zona vazia ou no limite da janela para a redimensionar de modo a caberem todas as categorias e frases.
- Arraste uma zona vazia da janela para a mover, e prima `Esc` para a ocultar.
- Aumente e reduza o zoom com `F12` e `F11` (consulte [Atalhos](#atalhos)).
- Clique com o botão direito num botão ou numa zona vazia (ou prima `Shift + F10` ou a tecla de menu) para abrir um menu com `Colar`, `Reduzir`, `Ampliar`, `Repor o zoom`, `Ajuda`, `Definições`, `Olhar / Clique por fixação...`, `Ocultar` e `Sair do SimonSays...`, cada um com o seu atalho. A primeira entrada mostra o título do quadro (`Quadro sem título` se não tiver); escolha-a para ver os créditos do quadro, o seu idioma e quantas categorias e frases contém. O menu só aparece enquanto `Mostrar botões e menu de acesso rápido` estiver ativo nas definições.
- Num botão de categoria ou de frase, o menu oferece ainda `Editar...`, `Mover para a esquerda`, `Mover para a direita`, `Adicionar nova categoria...` ou `Adicionar nova frase...` e `Eliminar categoria...` ou `Eliminar frase...` para esse botão, sem ter de o selecionar antes; numa categoria, `Exportar categoria...` pergunta se quer guardar só essa categoria ou o quadro inteiro. Os atalhos `F4` a `F8` são mostrados quando o botão é a sua seleção atual, porque essas teclas atuam sobre a seleção. Numa zona vazia, o menu oferece `Adicionar nova categoria...`, que acrescenta uma no fim; é também a forma de começar um quadro vazio.
- A faixa entre as categorias e as frases mostra um lembrete dos principais atalhos (ou a legenda do quadro, se tiver uma).

![O menu de contexto de um botão de frase](img/menu.png)

## Frases predefinidas
O SimonSays inclui um conjunto de categorias e frases pronto a usar em cada um dos seus 18 idiomas, ordenado para que as frases mais necessárias exijam menos seleções:

- 👋🏻 Saudações Frequentes
- 🗣️ Frases de Conversa
- 🚨 Frases de Emergência
- 🤲 Cuidado & Conforto
- 🏥 Saúde
- 🏠 Família & Casa
- 👥 Social
- ⏰ Hora & Data
- 💼 Frases de Negócios
- ✈️ Frases de Viagem
- 🔊 Sons

Cada idioma tem as suas próprias categorias e frases: quando muda o idioma nas definições (`F2`), a janela de categorias passa para o quadro desse idioma, e as alterações que fizer num idioma não afetam os outros.

As frases são ditas com as vozes instaladas no Windows. Se não houver nenhuma voz instalada para o seu idioma, as frases aparecem na mesma, mas podem ser ditas com outra voz (consulte [Resolução de problemas](#resolução-de-problemas)).

Pode alterar as frases predefinidas ou recuperá-las mais tarde; consulte [Editar categorias e frases](#editar-categorias-e-frases) e [Repor as frases predefinidas](#repor-as-frases-predefinidas).

## Atalhos
- `F1`: Abrir a ajuda
- `F2`: Abrir as definições
- `F3`: Definições de olhar / clique por fixação
- `Enter`: Dizer o texto da caixa de introdução
- `Esc`: Ocultar a janela de categorias
- `F11` / `Ctrl -`: Reduzir o zoom (janela de categorias)
- `F12` / `Ctrl +`: Aumentar o zoom (janela de categorias)
- `Ctrl + 0`: Repor o zoom
- `Shift + F10` ou a tecla de menu: Abrir o menu da janela de categorias (o mesmo que o clique com o botão direito)
- `Espaço` sobre `Reproduzir>`: Dizer o texto de novo, ou pará-lo enquanto toca

Os atalhos para editar categorias, frases e quadros estão em [Editar categorias e frases](#editar-categorias-e-frases).

## Definições (F2)
Abra as definições (`F2`) para controlar o seguinte. As definições têm duas colunas.

![A janela de definições](img/settings.png)

**Coluna da esquerda**
- **Texto predefinido**: ative ou desative `Inserir texto predefinido automaticamente` e personalize a frase predefinida.
  - `Mostrar teclado tátil`: abre o teclado tátil no ecrã quando a caixa de introdução se abre; fica ao lado da caixa de introdução, do lado oposto à barra de tarefas.
- `Mostrar botões e menu de acesso rápido`: mostra os botões 📂 (abrir um quadro) e ⚙️ (definições) junto ao botão `Categorias` da janela principal (consulte [Primeiros passos](#primeiros-passos)) e o menu de contexto da janela de categorias (consulte [Janela de categorias](#janela-de-categorias)). Desative-a para os ocultar todos e dar mais espaço à caixa de introdução.
- **Janela de categorias**:
  - `Minimizar automaticamente a janela de categorias` depois de selecionar.
  - `Lembrar o tamanho da janela de categorias`.
  - `Redimensionar automaticamente a janela de categorias`: ajusta a janela de categorias às suas categorias e frases sempre que são carregadas; por exemplo, ao iniciar, depois de mudar de idioma ou depois de uma importação.
- `Olhar / Clique por fixação...`: abre as definições de [Olhar / Clique por fixação](#olhar--clique-por-fixação-f3) por cima das definições, que continuam abertas com as suas alterações. As alterações que fizer aí são guardadas quando premir `OK` nessa janela, mesmo que depois prima `Cancelar` nas definições.

**Coluna da direita**
- **Idioma**: escolha o idioma da interface (English, Español, العربية, Euskara, Català, 简体中文, Français, Galego, Deutsch, עברית, हिन्दी, Italiano, 日本語, 한국어, Português, Português (Brasil), Русский, Valencià). Cada idioma tem também as suas próprias categorias e frases (consulte [Frases predefinidas](#frases-predefinidas)).
- **Voz**: escolha uma voz SAPI instalada que corresponda ao seu idioma.
- **Volume/Velocidade**: ajuste `Volume (10-100)` e `Velocidade (-10 a 10)`.
- `Testar voz`: ouça a voz selecionada (através do dispositivo escolhido em `Saída de som`).
- `Saída de som`: onde o SimonSays reproduz a sua voz e os seus sons: `Predefinição do Windows` (a escolha habitual) ou qualquer dispositivo ligado. Os cabos de áudio virtuais aparecem primeiro, que é o que precisa para as [reuniões online](#usar-o-simonsays-em-reuniões-online). Se o dispositivo escolhido não estiver ligado, o SimonSays reproduz pelo dispositivo predefinido do Windows e avisa-o uma vez; a escolha é mantida, aparece como `(não ligado)` e volta a ser usada assim que o dispositivo regressar.
- `Definições de escuta...`: abre os dispositivos de gravação do Windows, onde pode ativar **Escutar este dispositivo** para um cabo virtual (consulte [Usar o SimonSays em reuniões online](#usar-o-simonsays-em-reuniões-online)).
- **Comportamento da reprodução**:
  - `Falar a frase imediatamente ao selecionar` (dizer automaticamente com um clique).
  - `Parar reprodução anterior`: para automaticamente qualquer reprodução em curso antes de começar uma frase nova.
  - `Aumentar o volume do SimonSays ao reproduzir`: aumenta o volume do dispositivo por onde o SimonSays toca (consulte `Saída de som`) enquanto fala e repõe o nível anterior quando termina.
  - `Reduzir temporariamente outros áudios ao reproduzir`: baixa o volume das outras aplicações enquanto fala e repõe-no quando termina.

**Em baixo**
- `Ajuda` (canto inferior esquerdo, ou `F1` com as definições abertas) abre esta ajuda no idioma selecionado na lista `Idioma`; as definições continuam abertas com as suas alterações.
- `OK` guarda as alterações; `Cancelar` descarta-as.

Por predefinição, `Falar a frase imediatamente ao selecionar`, `Minimizar automaticamente a janela de categorias`, `Redimensionar automaticamente a janela de categorias`, `Aumentar o volume do SimonSays ao reproduzir` e `Reduzir temporariamente outros áudios ao reproduzir` estão ativos, e `Parar reprodução anterior` está desativado.

## Olhar / Clique por fixação (F3)
Ative frases e botões sem usar as mãos, mantendo o olhar (ou o cursor do rato) sobre eles durante um tempo definido. Esta funcionalidade é nova: precisa de testes e comentários; diga-nos como funciona com o seu dispositivo através da opção `Enviar comentários` do menu da área de notificação.

Configurações suportadas:
- **Qualquer software de controlo ocular que mova o cursor do rato** (Irisbond EasyClick, Tobii Dynavox Computer Control, os modos de cursor do Controlo ocular do Windows, OptiKey): o clique por fixação segue o cursor.
- **Leitura direta do olhar (sem necessidade de mover o cursor)**: Irisbond Hiru (modo HID) e os dispositivos Tobii através do seu próprio software; verificado com o Tobii Eye Tracker 4C, o Tobii Eye Tracker 5 e o Tobii Dynavox PCEye 5.
- **Tobii Eye Tracker 5**: o seu software atual só instala um motor de 64 bits, por isso o SimonSays lê-o através de um pequeno programa auxiliar, `SimonSaysGaze64.exe`, instalado ao seu lado. O auxiliar só se inicia quando é preciso, não tem janela, fecha-se com o SimonSays e mantém o olhar no seu computador: nunca é guardado nem enviado para lado nenhum.
- **Licença do Tobii Eye Tracker 4C e do Eye Tracker 5**: a licença de utilizador final da Tobii para estes dispositivos não permite a utilização médica, incluindo a utilização em hospitais e outros contextos de saúde. Os dispositivos Tobii Dynavox, como o PCEye 5, têm as suas próprias condições de licença.
- **Software do Tobii Eye Tracker 4C**: o SimonSays lê o 4C através do Tobii Core. Se ligar um Eye Tracker 5 ao mesmo computador, o Windows instala o Tobii Experience, que desativa o Tobii Core, e o 4C deixa de funcionar. Para voltar a usar o 4C, reinstale o Tobii Core; a versão recomendada é `Tobii_Eye_Tracking_Core_v2.13.1.7294_x86.exe`, da [página de introdução da Tobii](https://gaming.tobii.com/getstarted/).

Onde funciona o clique por fixação:
- Na barra do SimonSays, na janela de categorias e nos seus botões de frases e categorias.
- Nos botões das janelas e perguntas do próprio SimonSays: as definições, a janela Olhar / Clique por fixação, as janelas de adicionar, editar e pré-visualizar, e todas as confirmações, como `Sim` / `Não` ou `OK` / `Cancelar`. Estes botões estão no seu idioma e começam a responder à fixação cerca de três quartos de segundo depois de a janela se abrir, para que uma pergunta que apareça onde está a olhar não se possa responder sozinha.
- Quando o olhar é lido diretamente, o botão para onde está a olhar acende-se tal como acontece com o rato, para que veja o que vai selecionar antes de o progresso começar. Um piscar de olhos mantém o realce; desviar o olhar retira-o.

![A janela Olhar / Clique por fixação](img/dwell.png)

Abra as definições de Olhar / Clique por fixação (`F3`, o menu da área de notificação ou o botão `Olhar / Clique por fixação...` nas definições) para configurar:
- **Modo de ativação**: escolha uma das seguintes opções:
  - `Automático`: deteta os dispositivos presentes e ativa o clique por fixação com o cursor do rato ou com a deteção do olhar, conforme o caso. O software da Tobii em segundo plano (sempre em execução depois de instalado) não basta para ativar a fixação com o cursor; um dispositivo Tobii só é usado enquanto está realmente a seguir o olhar.
  - `Cursor do rato`: ativa sempre o clique por fixação com o cursor do rato.
  - `Dispositivo de seguimento ocular HID`: ativa sempre o clique por fixação com a deteção do olhar (Irisbond Hiru em modo HID, ou dispositivos Tobii). Só o seu olhar ativa os botões: enquanto não chegar nenhum olhar (seguimento desligado ou dispositivo desligado), nada é ativado por fixação e o rato funciona normalmente.
  - `Desativado`: desativa sempre o clique por fixação.
  - **Atraso de fixação inicial**: quanto tempo, em milissegundos, o seu olhar (ou o cursor) tem de repousar num botão antes de a barra de progresso começar (150 por predefinição). Assim, passar o olhar pelos botões não mostra nenhum progresso; o botão é ativado após o atraso de fixação inicial mais o tempo de fixação. Defina 0 para o progresso começar de imediato.
  - **Tempo de fixação**: a duração (em milissegundos) necessária para fazer clique ao olhar para um elemento (500 por predefinição). Enquanto a fixação dura, é mostrado um indicador de progresso.
  - **Raio de tolerância**: a tolerância do olhar (em píxeis) que permite pequenos movimentos do olho sem interromper a fixação.
  - **Tempo de espera**: milissegundos após uma ativação durante os quais o clique por fixação não pode voltar a ser acionado (evita cliques repetidos por engano).
  - **Cor de progresso**: a cor do indicador de progresso da fixação (por predefinição, a cor de destaque do Windows).
  - **Afastar-se antes de a fixação voltar a ativar** (ativo por predefinição): depois de um clique por fixação, nenhum botão pode voltar a ser ativado por fixação até o ponteiro ou o olhar se afastar desse ponto (além do raio de tolerância), ou até desviar o olhar por um momento. Ficar sobre um botão ativa-o uma única vez, por isso uma frase longa não se repete antes de terminar. Os cliques do rato não são afetados.
  - **Detetar como usa o dispositivo**: dois botões que ajudam a aplicação a detetar se move o cursor do rato com um dispositivo de seguimento ocular ou se simplesmente clica nos botões.
  - **Detetado**: mostra o tipo de dispositivo detetado e mais informações sobre o software relacionado.
  - `Guardar relatório de diagnóstico...` (por baixo de Detetado): escreve um relatório sobre o seu dispositivo de seguimento ocular e o respetivo software em `hid_dump.txt` e abre a pasta, para que o possa enviar ao suporte.
  - Um botão `Repor` (em baixo à esquerda) repõe todas as definições de fixação nos valores predefinidos.
  - Um botão `OK` guarda as alterações; `Cancelar` descarta-as.

## Usar o SimonSays em reuniões online
O SimonSays pode falar diretamente numa videochamada (Teams, Zoom, Google Meet…), de modo que os outros participantes ouvem as suas frases como se viessem do seu microfone. Precisa de um cabo de áudio virtual, como o gratuito [VB-Audio Virtual Cable](https://vb-audio.com/Cable/):

1. Instale o cabo de áudio virtual e reinicie o Windows se lhe for pedido.
2. Envie o som do SimonSays para o cabo: abra as definições (`F2`) e escolha `CABLE Input` em `Saída de som` (os cabos virtuais aparecem primeiro). Prima `Testar voz` para confirmar: já não o deverá ouvir pelos altifalantes, porque agora vai para o cabo.
3. Na sua aplicação de reuniões, escolha `CABLE Output` como microfone.
4. Para ouvir também o SimonSays, prima `Definições de escuta...` nas definições. Abrem-se os dispositivos de gravação do Windows: selecione `CABLE Output`, abra **Propriedades → Escutar**, marque **Escutar este dispositivo**, escolha os seus altifalantes ou auscultadores em **Reproduzir através deste dispositivo** e prima `OK`. Só precisa de o fazer uma vez.

O SimonSays não consegue ativar **Escutar este dispositivo** por si: é uma definição do lado de gravação do cabo que o Windows só deixa que seja o utilizador a alterar, por isso `Definições de escuta...` leva-o diretamente à janela certa.

Convém saber:
- Deixe o dispositivo de reprodução predefinido do Windows nos seus altifalantes ou auscultadores, não em `CABLE Input`. Se **Escutar este dispositivo** reproduzir em "Dispositivo de reprodução predefinido" enquanto esse dispositivo predefinido for o cabo, o som volta a entrar no cabo.
- Se na reunião se ouvirem as suas frases mas não os efeitos sonoros (aplausos, uma campainha), é a supressão de ruído da aplicação de reuniões que os remove; reduza ou desative a supressão de ruído na aplicação.
- Se o cabo for desinstalado ou desligado, o SimonSays reproduz pelo dispositivo predefinido do Windows e avisa-o uma vez; volta ao cabo assim que este estiver de novo disponível.
- Para ter frases prontas para uma chamada, acrescente a categoria **Modelo de reunião**: prima `F9` ou o botão 📂 e abra o quadro com o nome no seu idioma (`SimonSays_Modelo_de_reunião_(Português).ssc` em português). Tem cumprimentos, "Consegue ouvir-me?", uma breve explicação de que fala através de um programa, sim / não, concordo / não concordo, pedir para repetir ou falar mais devagar, e despedidas. Edite-o para acrescentar frases para a sua reunião.
- Em alternativa ao passo 2, pode continuar a encaminhar o SimonSays com a definição por aplicação do Windows (**Definições → Sistema → Som → Misturador de volume**), deixando `Saída de som` em `Predefinição do Windows`. O SimonSays só aparece nessa lista depois de ter reproduzido algo.

## Menu do ícone da área de notificação
- `Mostrar` / `Ocultar` a interface principal
- `Definições`
- `Olhar / Clique por fixação...`
- `Web` abre a página web do SimonSays
- `Sobre` mostra a versão e os créditos
- `Enviar comentários` abre um [formulário de comentários](https://forms.gle/KMbpEDYmwnFJUhgy8) no navegador predefinido
- `Sair` fecha o SimonSays, depois de perguntar

Pode mover a janela de contexto da área de notificação e outras caixas de diálogo arrastando a sua barra de título; redimensione a janela de categorias arrastando os seus limites, e ative `Lembrar o tamanho da janela de categorias` nas definições para manter o tamanho.

## Editar categorias e frases
Esta secção e as seguintes destinam-se a cuidadores, terapeutas da fala e utilizadores avançados que alteram as categorias, frases, sons e aspeto de um quadro. Cada alteração é guardada de imediato (consulte [Anular alterações](#anular-alterações)).

### Que editor usar
- **No SimonSays**: alterações do dia a dia; mudar o nome, adicionar, mover ou eliminar categorias e frases, dar-lhes um emoji, adicionar sons, colar uma lista de frases ou ajustar uma cor. O resto desta secção explica como.
- **No [SimonSays Web](https://github.com/juan-rey/simonsays-web)**: trabalho maior com quadros, que lá é muito mais fácil: quadros feitos com pictogramas de CAA (pode pesquisar na biblioteca de pictogramas ARASAAC), imagens de ícone personalizadas e quadros com estilo, com cores, tamanhos e disposições. É especialmente recomendado para criar um quadro **Fitzgerald Key**, em que cada grupo de palavras tem a sua própria cor. É um editor visual que abre e guarda quadros `.ssc`/`.ssz` diretamente no navegador, sem os carregar para a internet; quando terminar, importe o quadro no SimonSays com `F9` ou com o botão 📂. É também a forma de usar os quadros do SimonSays noutras plataformas.

Atalhos de edição (janela de categorias):
- `F4`: Editar a seleção atual
- `F5` / `F6`: Ir para a seleção anterior / seguinte nas listas
- `F7`: Adicionar depois da seleção atual (categoria ou frase, conforme o contexto)
- `F8`: Eliminar a seleção atual
- `F9`: Importar categorias
- `F10`: Exportar categorias
- `Ctrl + F4`: Editar o estilo do quadro (consulte [Personalizar o aspeto](#personalizar-o-aspeto-estilos-do-quadro-e-das-categorias))
- `Ctrl + F8`: Eliminar todas as categorias e as suas frases (pede confirmação duas vezes)
- `Ctrl + V` ou `Shift + Insert`: Adicionar o texto copiado como frases, uma por parágrafo (consulte [Gerir frases](#gerir-frases))
- Arrastar um ficheiro `.txt`, `.ssc` ou `.ssz` para a janela: adiciona um `.txt` como nova categoria, ou importa um quadro tal como com `F9` (consulte [Gerir categorias](#gerir-categorias))

> **Sugestão:** exporte o seu quadro (`F10`) antes de usar `Ctrl + F8` ou de importar um quadro completo (`F9`), para o poder recuperar.

### Gerir categorias
- Mudar o nome: selecione a categoria → (atalho: `F4`) `Alterar nome da categoria` → altere o nome → `OK`.
- Mover a seleção: `F5`/`F6` para ir para o elemento anterior/seguinte ao percorrer as listas.
- Adicionar: selecione uma categoria → (atalho: `F7`) janela `Adicionar nova categoria` → escreva o nome → `OK`.
- Eliminar: selecione a categoria → (atalho: `F8`) `Eliminar categoria`; confirme a eliminação da categoria e das suas frases.
- Adicionar a partir de um ficheiro de texto: arraste um ficheiro `.txt` para a janela de categorias → é criada uma categoria com o nome do ficheiro e uma frase por parágrafo (os parágrafos são separados por uma linha em branco) → reveja a pré-visualização → `OK`. Pode largar vários ficheiros de uma vez; cada um tem a sua própria pré-visualização. Um ficheiro cujo nome coincida com uma categoria existente é recusado.
- Abrir um quadro arrastando-o: arraste um ficheiro `.ssc` ou `.ssz` para a janela de categorias → o SimonSays pergunta se o quer importar; com `Sim` é importado tal como com `F9` (consulte [Importar e exportar categorias](#importar-e-exportar-categorias)). Se o ficheiro contiver um quadro completo, substitui o atual, e antes é guardada uma cópia do quadro atual como `backup.ssz`. Pode largar ficheiros de texto e de quadro ao mesmo tempo; cada um é tratado à vez. Abrir um ficheiro `.ssc` ou `.ssz` a partir do Explorador de Ficheiros (duplo clique) faz a mesma pergunta.

#### Definir ou alterar o ícone de uma categoria
Cada categoria pode mostrar um emoji ou um ficheiro de imagem personalizado (`.ico`, `.png` ou `.jpg`) no seu botão. Indique o ícone na janela de adicionar ou editar com o prefixo separador `##`:

```
<ícone>##<nome da categoria>
```

- **Emoji**: escreva o emoji (ou os emojis) seguido de `##` e do nome.
  - Exemplo: `🙂##Saudações`
- **Ficheiro de ícone** (`.ico`, `.png`, `.jpg`): escreva o nome do ficheiro ou o caminho completo seguido de `##` e do nome. Um nome de ficheiro sem pasta é procurado como descrito em [Ordem de pesquisa dos ficheiros de som e de ícone](#ordem-de-pesquisa-dos-ficheiros-de-som-e-de-ícone).
  - Exemplo: `C:\icons\smile.png##Saudações`
- **Sem ícone**: escreva apenas o nome da categoria, sem o prefixo `##`.
  - Exemplo: `Saudações`

> Nota: para ícones a partir de ficheiro só são suportados ficheiros `.ico`, `.png` e `.jpg`. As outras extensões são ignoradas e não é mostrado nenhum ícone. As imagens PNG mantêm a transparência; as imagens muito grandes são reduzidas automaticamente ao carregar.

### Gerir frases
- Editar: selecione a frase → (atalho: `F4`) `Editar frase` → atualize o texto → `OK`.
- Mover a seleção: `F5`/`F6` para ir para a frase anterior/seguinte.
- Adicionar: selecione uma frase → (atalho: `F7`) `Adicionar nova frase` → escreva o texto → `OK`.
- Eliminar: selecione a frase → (atalho: `F8`) `Eliminar frase`; confirme.
- Adicionar várias de uma vez: copie um texto com as frases separadas por uma linha em branco → selecione uma frase (ou uma categoria) → `Ctrl + V` → reveja a pré-visualização → `OK`. As frases são adicionadas depois da frase selecionada, ou no fim da categoria. As quebras de linha dentro de um parágrafo passam a espaços, e os marcadores de ícone `##` e de áudio `::` funcionam como na janela `Adicionar nova frase`.
- Adicionar sons: arraste um ou mais ficheiros `.wav` ou `.mp3` para a janela de categorias → reveja a pré-visualização → `OK`. Cada ficheiro passa a ser uma frase de som com o nome do ficheiro, adicionada depois da frase selecionada (ou no fim da categoria selecionada), e o ficheiro é copiado para a pasta de recursos do quadro para que o acompanhe quando o exportar. Um ficheiro existente nunca é substituído: se já existir um som diferente com o mesmo nome, o novo é guardado como `nome (2).wav`.

#### Definir ou alterar o ícone de uma frase
As frases seguem a mesma convenção de prefixo de ícone que as categorias, com o separador opcional `##` antes do texto da frase:

```
<ícone>##<texto da frase>
```

- **Emoji**: `👍##Parece-me bem!`
- **Ficheiro de ícone**: `C:\icons\check.png##Parece-me bem!`
- **Sem ícone**: `Parece-me bem!`

As frases podem incluir um ficheiro de áudio (consulte [Misturar voz e sons](#misturar-voz-e-sons)). Para combinar ícone, texto e áudio, use o formato:

```
<ícone>##<texto da frase>::<ficheiro de áudio>
```

- Exemplo: `🔔##Atenção::alert.mp3`

O prefixo `##` e o sufixo `::` são opcionais e independentes; pode usar qualquer combinação:
- Só prefixo de ícone: `🔔##Atenção`
- Só sufixo de áudio: `Atenção::alert.mp3`
- Ambos: `🔔##Atenção::alert.mp3`
- Nenhum: `Atenção`

## Misturar voz e sons
- Use o delimitador `♫` para inserir nomes de ficheiros de áudio no texto:
  - Exemplo: `Olá ♫doorbell.mp3♫, aguarde um momento.`
  - O texto fora dos delimitadores é dito; o texto dentro deles é tratado como um ficheiro de som e reproduzido nesse ponto.
- Áudio suportado: `.wav` e `.mp3`. Os ficheiros MIDI (`.mid`, `.midi`) são aceites numa frase, mas não tocam; converta-os para `.wav` ou `.mp3`.
- Use caminhos absolutos ou relativos a que o programa consiga aceder. Garanta que os delimitadores estão aos pares, para que o texto final não seja tratado como áudio.
- Para escrever `♫`, copie-o desta ajuda, ou selecione uma frase que tenha um som: o seu marcador `♫…♫` é colocado na caixa de introdução, onde pode acrescentar texto à volta.

### Sons incluídos no SimonSays
Estes sons são instalados com o SimonSays e podem ser usados pelo nome em qualquer texto ou frase; por exemplo, `♫laugh.mp3♫` na caixa de introdução, ou `Muito bem!::applause.mp3` para um botão de frase com a legenda "Muito bem!" que reproduz aplausos:

`ahem.mp3`, `alert.mp3`, `applause.mp3`, `aww.mp3`, `beep.mp3`, `boo.mp3`, `call_bell.mp3`, `censor.mp3`, `doorbell.mp3`, `drumroll.mp3`, `evil_laugh.mp3`, `fail.mp3`, `hmmmm.mp3`, `kisses.mp3`, `laugh.mp3`, `mmm-hmm.mp3`, `nuh-uh.mp3`, `phone_ring.mp3`, `rimshot.mp3`, `scream.mp3`, `shots.mp3`, `sigh.mp3`

### Ordem de pesquisa dos ficheiros de som e de ícone
Quando o nome de um ficheiro de som ou de ícone não contém uma letra de unidade nem um caminho completo, o SimonSays procura-o por esta ordem e usa a primeira correspondência:

1. **A subpasta de recursos do quadro atual**: se o estilo do quadro definir `title` ou `resource-folder`, a sua própria subpasta dentro da pasta de recursos (consulte [Personalizar o aspeto](#personalizar-o-aspeto-estilos-do-quadro-e-das-categorias)).
2. **Pasta de recursos**: `%LocalAppData%\SimonSays\resources\` (o local predefinido para os ícones e sons dos quadros; os pacotes `.ssz` importados instalam aqui os seus ficheiros).
3. **Pasta de dados do SimonSays**: `%LocalAppData%\SimonSays\` (mantida como alternativa para ficheiros que versões anteriores aí colocavam, antes de existir a pasta de recursos).
4. **Diretório de trabalho**: o diretório de trabalho atual do processo (só é pesquisado se for diferente da pasta do executável).
5. **Pasta do executável**: a pasta que contém `SimonSays.exe`.

Se o ficheiro não for encontrado em nenhum destes locais, é usado o som de recurso incorporado (um ícone simplesmente não é mostrado).

> **Sugestão:** recomendamos que coloque os seus ficheiros de som e de ícone em `%LocalAppData%\SimonSays\resources\`; sobrevivem às atualizações da aplicação e são próprios de cada utilizador. Um quadro com um estilo `title` ou `resource-folder` recebe aí automaticamente a sua própria subpasta, o que separa os seus ficheiros dos de outros quadros.

## Personalizar o aspeto (estilos do quadro e das categorias)
Pode alterar as cores, o tamanho e o estilo da janela de categorias e dos seus botões; por exemplo, para criar quadros de comunicação com código de cores como o Fitzgerald Key. Os estilos são opcionais: sem eles, a janela mantém o aspeto predefinido.

> **Sugestão:** tudo o que se segue pode ser escrito no SimonSays, o que é prático para pequenos retoques. Para criar um quadro inteiro com estilo ou com pictogramas, recomenda-se o [SimonSays Web](https://github.com/juan-rey/simonsays-web): vê o resultado enquanto trabalha e ele escreve estes estilos por si (consulte [Que editor usar](#que-editor-usar)).

Um estilo é uma lista de pares `propriedade:valor;`. Os valores podem ser:
- **Cores**: hexadecimal `#RRGGBB`, por exemplo `#FFD966`.
- **Tamanhos**: um número de píxeis (`96`) ou uma percentagem do predefinido (`120%`).
- **Palavras-chave**: por exemplo `left`, `right`, `top`, `bottom`, `smart`, `normal`, `bold`.
- **Texto**: um nome de tipo de letra, ou o texto da legenda, do título ou dos créditos.

Tudo o que a aplicação não reconhece (uma propriedade mal escrita ou um valor errado) é simplesmente ignorado, por isso um estilo nunca estraga a janela.

### Dar estilo a uma categoria (janela Editar)
Selecione uma categoria e prima `F4` (Editar); depois acrescente `::` e um estilo a seguir ao nome:

```
<ícone>##<nome da categoria>::<propriedade>:<valor>;<propriedade>:<valor>;
```

- Exemplo: `🧍##Pessoas::background:#FFD966;` pinta de amarelo o botão Pessoas. A cor da categoria **aplica-se também às frases que contém**, por isso uma só linha colore todo um grupo de palavras: a base de um quadro Fitzgerald Key.
- Um estilo de categoria pode definir `background` e `text-color` para o botão da categoria, e qualquer propriedade de frase (ver abaixo) com o prefixo `phrase-`, por exemplo `phrase-corner-radius:8;`.

### Dar estilo a todo o quadro (`$$board`)
O aspeto geral (o fundo da janela, os separadores e os valores predefinidos de todos os botões de categoria e de frase) é um **estilo do quadro**. É guardado com o nome de categoria reservado `$$board` (não pode usar nomes com `$$` para as suas categorias).

- **Edite-o** premindo `Ctrl + F4` na janela de categorias. O editor mostra um `propriedade:valor;` por linha. Ao confirmar com `OK`, o novo estilo é aplicado e o SimonSays pergunta "Deseja reverter as alterações?": escolha `OK` para voltar ao estilo anterior, ou `Cancelar` para manter o novo.
- **Obtenha um** importando um ficheiro de categorias com estilo (`.ssc` / `.ssz`). Quando importa um ficheiro que contém um estilo do quadro, o SimonSays pergunta antes de substituir o seu e mostra o `title` e os `credits` do quadro (se existirem) na mensagem de importação. Um ficheiro cujo estilo do quadro só define `title`, `credits`, `language` ou `resource-folder` não altera nada visível, por isso o SimonSays não pergunta: se contiver uma só categoria, é adicionada e o seu aspeto mantém-se; se contiver um quadro completo, substitui o seu quadro, incluindo o aspeto, como qualquer quadro completo.

### Propriedades de estilo
- **Toda a janela**: `background`, `separator-color`, `text-color`, `font-name`, `font-size`, `text-weight`, `caption` (legenda mostrada entre os separadores), `title` e `credits` (mostrados ao importar o quadro), `language` (o idioma para o qual o quadro foi feito, por exemplo `language:Portuguese`; o SimonSays recusa importar o quadro enquanto o idioma definido for outro, e diz-lhe para que idioma deve mudar nas definições, `F2`).
- **Todos os botões de categoria** (prefixo `category-`) e **todos os botões de frase** (prefixo `phrase-`): `background`, `text-color`, `width`, `height`, `corner-radius`, `border-width`, `margin`, `icon-position` (`left`/`right`/`top`/`bottom`/`center`), `icon-size`, `font-name`, `font-size`, `text-weight`, `text-layout`.

Com `icon-position` em `center`, o ícone fica no centro do botão e o texto é ocultado (uma tecla só com ícone); use `top` se quiser o ícone em cima e o nome por baixo. Se quiser texto sobre um ícone centrado, acrescente um `text-layout` explícito (por exemplo `text-layout:bottom`) e ele será mostrado nessa posição.

`text-layout` aceita uma palavra-chave horizontal (`left`/`center`/`right`) e/ou uma vertical (`top`/`middle`/`bottom`), ou `smart`. A predefinição é smart: o texto é centrado no espaço que o ícone deixa livre (ícone em cima → legenda centrada por baixo; centrado em todo o botão quando não há ícone).

### Exemplo: um quadro Fitzgerald Key com código de cores
Para colorir seis grupos de palavras, edite cada categoria (`F4`) e dê-lhe uma cor de fundo. Estas são as cadeias exatas que escreve na janela de edição:

```
🧍##Pessoas::background:#FFD966;
🏃##Ações::background:#93C47D;
📏##Descritivos::background:#6FABDC;
🍎##Substantivos::background:#F6B26B;
🕒##Diversos::background:#FFFFFF;
👋##Social::background:#B4A7D6;
```

Cada cor colore também as frases dessa categoria, por isso todo o quadro fica com código de cores. Os retoques de todo o quadro (como pôr o emoji de cada categoria em cima com o nome por baixo, e colorir os separadores) vêm de um estilo do quadro (`Ctrl + F4`) como:

```
category-icon-position:top;separator-color:#B4A7D6;
```

Para um quadro Fitzgerald Key completo, com pictogramas em cada tecla e as frases organizadas por grupos de palavras, crie-o no [SimonSays Web](https://github.com/juan-rey/simonsays-web) e importe-o com `F9`.

Os estilos acompanham as suas categorias: a exportação (`F10`) inclui-os e a importação (`F9`) trá-los consigo, por isso um quadro com estilo pode ser partilhado como um único ficheiro.

## Importar e exportar categorias
Os quadros são partilhados como ficheiros `.ssc` (categorias e frases) ou `.ssz` (o mesmo, mais os ficheiros de som e de ícone que usam, num único ficheiro). Também pode criá-los e editá-los visualmente no navegador com o [SimonSays Web](https://github.com/juan-rey/simonsays-web).

- **Exportar** (`F10`): quando lhe for perguntado, escolha exportar todas as categorias ou só a selecionada; a janela de guardar abre-se por predefinição em `%LocalAppData%\SimonSays\boards\`, embora possa guardar em qualquer lado. O SimonSays guarda automaticamente um `.ssz` quando as suas categorias ou frases usam ficheiros de som ou de ícone que consegue encontrar (os sons `.mid`/`.midi` não são incluídos). Exportar todas as categorias inclui o estilo do quadro; exportar uma categoria inclui só o estilo dessa categoria. As mensagens de sucesso ou de erro aparecem no idioma selecionado.
- **Importar** (`F9`): escolha um ficheiro `.ssc` ou `.ssz`.
  - Um ficheiro com **várias categorias ou com um estilo do quadro** é um quadro completo: **substitui todas as categorias e frases do idioma atual**. Antes, o seu quadro anterior é guardado como `backup.ssz` (consulte [Anular alterações](#anular-alterações)).
  - Um ficheiro com **uma só categoria** é adicionado ao seu quadro; se já existir uma categoria com o mesmo nome, ser-lhe-á perguntado se a quer substituir. O mesmo acontece quando o ficheiro só dá nome ao seu quadro (`title`, `credits` ou `language`, sem cores, tamanhos nem tipos de letra): esses dados nunca substituem o seu quadro nem o seu aspeto.
  - Os ficheiros de som e de ícone de um `.ssz` são instalados na pasta de recursos, e a janela de categorias ajusta-se ao quadro importado. As mensagens de sucesso ou de erro aparecem no idioma selecionado.

## Anular alterações
O SimonSays guarda cada alteração de imediato e não tem um comando geral de Anular. Estas são as formas de voltar atrás:

- **Anular uma importação**: antes de uma importação substituir o seu quadro, o SimonSays guarda o quadro anterior como `backup.ssz` em `%LocalAppData%\SimonSays\boards\`. Para anular a importação, prima `F9` e escolha `backup.ssz`: o seu quadro anterior volta tal como estava, categorias e aspeto, sem mais perguntas. Só é guardado o último quadro substituído: cada nova importação de um quadro completo substitui a cópia.
- **Anular uma alteração ao estilo do quadro**: depois de editar o estilo do quadro com `Ctrl + F4`, o SimonSays pergunta "Deseja reverter as alterações?"; escolha `OK` para voltar ao estilo anterior.
- **Anular uma alteração a uma categoria ou frase**: as alterações feitas com `F4`, `F7` ou `F8` não podem ser anuladas automaticamente. Antes de fazer grandes alterações a um quadro que lhe interessa, exporte-o (`F10`); importar esse ficheiro mais tarde (`F9`) leva-o de volta a esse ponto.

### Repor as frases predefinidas
O SimonSays nunca substitui as suas próprias categorias e frases, por isso as novas frases predefinidas de uma atualização só aparecem nos idiomas que ainda não usou. Pode voltar às categorias e frases incorporadas em qualquer altura; por exemplo, para recomeçar ou para receber as frases novas de uma atualização. Isto **substitui todo o quadro do idioma atual**:

1. Clique no botão 📂 junto a `Categorias` (ou prima `F9` na janela de categorias). Abre-se a pasta de quadros.
2. Escolha o quadro de frases predefinidas do seu idioma; em português, `SimonSays_frases_predefinidas_(Português).ssc` (nos outros idiomas tem a mesma forma, nesse idioma; por exemplo, `SimonSays_default_phrases_(English).ssc`).
3. Confirme a importação. Antes, o seu quadro atual é guardado como `backup.ssz`, por isso ainda o pode recuperar (consulte [Anular alterações](#anular-alterações)).

Este quadro contém sempre as frases predefinidas da versão do SimonSays que está a usar: o SimonSays atualiza-o na pasta de quadros ao iniciar e ao mudar de idioma. Se quiser manter o seu próprio quadro, exporte-o antes (`F10`) com o nome que preferir; o `backup.ssz` é substituído na importação seguinte.

Se o ficheiro não estiver na pasta de quadros, resta um último recurso: saia do SimonSays (menu da área de notificação → `Sair`), abra o Editor de Registo (`regedit`), elimine a chave com o nome em inglês do seu idioma (por exemplo `Portuguese`) em `HKEY_CURRENT_USER\SOFTWARE\SimonSays\Phrases\` e inicie de novo o SimonSays. As categorias e frases predefinidas desse idioma são criadas de novo, sem cópia de segurança.

Para `Portuguese (Brazil)`: se também existir uma chave `Portuguese`, o SimonSays copia esse quadro para `Portuguese (Brazil)` em vez de criar as frases predefinidas brasileiras, por isso elimine também a chave `Portuguese` (exporte-a antes se a usar).

## Onde o SimonSays guarda os seus ficheiros
- **As categorias, as frases e as definições** são guardadas no registo do Windows, em `HKEY_CURRENT_USER\SOFTWARE\SimonSays` (o quadro de cada idioma em `Phrases\<Language>`).
- `%LocalAppData%\SimonSays\resources\`: os seus ficheiros de som e de ícone, e os instalados a partir de quadros `.ssz`; um quadro cujo estilo define `title` ou `resource-folder` tem a sua própria subpasta.
- `%LocalAppData%\SimonSays\boards\`: a pasta predefinida dos quadros exportados, a cópia automática `backup.ssz` e os quadros que vêm com o SimonSays: as frases predefinidas de cada idioma, e categorias prontas que pode acrescentar ao seu quadro com `F9` ou com o botão 📂: Sim / Não, Dor e corpo, Soletrar, Jogos, Perguntas, Cuidados diários, um Modelo de reunião e um Teste de audição, com o nome no seu idioma, e `Spelling_NATO`, `Alert_tones` e `Morse_code` para qualquer idioma. Cada uma acrescenta uma categoria sem substituir o seu quadro. Os quadros do seu idioma são copiados para lá ao iniciar e quando muda o idioma nas definições, sempre que o quadro predefinido do seu idioma falte ou tenha mudado, substituindo as cópias da pasta; por isso, guarde com outro nome os quadros que editar.
- `%LocalAppData%\SimonSays\debug\`: ficheiros de diagnóstico (como `hid_dump.txt`) que o suporte lhe pode pedir.

Para abrir uma destas pastas, cole o seu caminho (por exemplo `%LocalAppData%\SimonSays`) na barra de endereço do Explorador de Ficheiros.

## Resolução de problemas
- **A inicialização da voz falha**: verifique se o SAPI está instalado e se existe uma voz compatível.
- **Não há voz para o meu idioma, ou as frases são ditas com outro sotaque**: o SimonSays usa as vozes instaladas no Windows. Adicione uma voz para o seu idioma nas definições do Windows (**Hora e idioma → Voz → Gerir vozes**) e escolha-a depois nas definições do SimonSays (`F2`). Algumas vozes "naturais" mais recentes do Windows podem não estar disponíveis para outras aplicações.
- **Os sons não tocam**: confirme o caminho e a extensão do ficheiro, e que os delimitadores `♫` estão aos pares.
- **A barra de tarefas já não se oculta automaticamente com o SimonSays aberto**: é intencional. O SimonSays está na barra de tarefas, por isso mantém-na visível enquanto está em execução e volta a ativar "Ocultar automaticamente a barra de tarefas" quando sai. Se o SimonSays foi fechado inesperadamente (por exemplo, por uma falha de energia), a opção volta a ser ativada da próxima vez que sair do SimonSays.
- **O botão Widgets tapa a barra do SimonSays** (Windows 11): desative os Widgets nas **Definições → Personalização → Barra de tarefas** do Windows. O SimonSays oferece-se para abrir essa página uma vez, da primeira vez que deteta os Widgets.
- **A janela de categorias é demasiado pequena ou demasiado grande**: faça duplo clique numa zona vazia ou no limite da janela para a ajustar ao conteúdo, ou use o zoom com `F11` / `F12`.
- **As minhas categorias ou frases desapareceram**: verifique o idioma nas definições (`F2`); cada idioma tem o seu próprio quadro. Se uma importação substituiu o seu quadro, consulte [Anular alterações](#anular-alterações).
- **"O SimonSays não conseguiu guardar as suas categorias e frases"**: a sua alteração continua a ser mostrada, mas pode perder-se ao fechar o SimonSays. Exporte o seu quadro com `F10` de imediato para guardar uma cópia; se voltar a acontecer, comunique-o. A mensagem aparece uma só vez, não a cada alteração, para nunca atrapalhar o uso do quadro.
- **Erros de importação ou exportação**: verifique as permissões do ficheiro e a localização no disco; tente de novo.
- **As alterações de idioma ou de voz não são aplicadas**: reabra as definições e confirme a seleção. Se as alterações continuarem a não ser aplicadas, reinicie o SimonSays e tente de novo.
- **Comunicar problemas com o dispositivo de seguimento ocular**: se o suporte lhe pedir um diagnóstico, abra a janela Olhar / Clique por fixação (`F3`) e clique em `Guardar relatório de diagnóstico...`. O relatório (`hid_dump.txt`) é guardado em `%LocalAppData%\SimonSays\debug\` e essa pasta abre-se. Só é criado quando o pede.

## Comentários, suporte e privacidade
- A janela `Sobre` (menu da área de notificação) mostra a versão, a descrição e os direitos de autor.
- Partilhe sugestões, problemas ou como o SimonSays funciona com o seu dispositivo de seguimento ocular através da opção `Enviar comentários` do menu da área de notificação, que abre um [formulário de comentários](https://forms.gle/KMbpEDYmwnFJUhgy8).
- Para comunicar erros e obter suporte técnico, visite o [repositório do SimonSays no GitHub](https://github.com/juan-rey/SimonSays).
- O SimonSays é livre e de código aberto, com a Open Software License 3.0 (OSL-3.0). Não mostra publicidade e não recolhe os seus dados.
