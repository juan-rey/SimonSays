# Guia de Ajuda do SimonSays

## Visão geral
SimonSays é um assistente de texto para fala (TTS) para Windows, criado por Juan Rey Saura, otimizado para fala rápida, bibliotecas de frases e reprodução de sons em linha. Suporta vários idiomas, vozes personalizáveis e funcionalidades práticas para uma comunicação eficiente.

SimonSays utiliza o espaço livre à esquerda da barra de tarefas do Windows, proporcionando acesso rápido a uma dezena de categorias de frases de uso frequente (e personalizáveis) com apenas alguns cliques.

## Atalhos
- `F1`: Abrir Ajuda.
- `F2`: Abrir Definições.
- `F3`: Adicionar após a seleção atual (contexto categoria/frase).
- `F4`: Editar a seleção atual.
- `F5` / `F6`: Mover para a seleção anterior / seguinte nas listas.
- `F8`: Eliminar a seleção atual.
- `F9`: Importar categorias.
- `F10`: Exportar categorias.
- Botão `Reproduzir>`: Iniciar reprodução.

## Início e utilização básica
1. Inicie o SimonSays; aparece na área de notificação do Windows.
2. Para falar, pode:
   - Escrever texto personalizado na caixa de entrada e premir o botão `Reproduzir>` (ou `Enter`).
   - Clicar numa ou mais frases guardadas na janela de Categorias.
3. Durante a reprodução, o botão mostra `>` para indicar reprodução ativa. Pode premir novamente para parar instantaneamente.

## Janela de Categorias (frases)
- Abra com o botão `Categorias`. Redimensione ou recorde o tamanho através das Definições.
- Selecione uma categoria em cima e frases em baixo.
- Um clique numa frase reproduz automaticamente se `Falar a frase imediatamente ao selecionar` estiver ativado; caso contrário, carrega a frase na caixa de entrada principal sem falar.

### Gerir categorias
- Adicionar: selecione categoria → (atalho: `F3`) diálogo `Adicionar nova categoria` → introduza o nome → `OK`.
- Renomear: selecione categoria → (atalho: `F4`) `Alterar nome da categoria` → altere o nome → `OK`.
- Mover seleção: `F5`/`F6` para o item anterior/seguinte.
- Eliminar: selecione categoria → (atalho: `F8`) `Eliminar categoria`; confirme a eliminação da categoria e das suas frases.

### Gerir frases
- Adicionar: selecione frase → (atalho: `F3`) `Adicionar nova frase` → introduza o texto → `OK`.
- Editar: selecione frase → (atalho: `F4`) `Editar frase` → atualize o texto → `OK`.
- Mover seleção: `F5`/`F6` para a frase anterior/seguinte.
- Eliminar: selecione frase → (atalho: `F8`) `Eliminar frase`; confirme.

## Definições (F2)
Abra as Definições (`F2`) para controlar:
- **Texto predefinido**: ativar/desativar `Inserir texto predefinido automaticamente` e personalizar a frase predefinida.
  - `Mostrar teclado tátil` — abre o teclado tátil no ecrã quando a caixa de entrada recebe o foco, posicionado perto da janela principal.
- **Idioma da interface**: escolher idioma da UI (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Voz**: selecionar voz SAPI instalada de acordo com a sua preferência.
- **Volume/Velocidade**: ajustar `Volume (10-100)` e `Velocidade (-10 a 10)`.
- `Testar voz`: ouvir a voz selecionada.
- **Janela de categorias**:
  - `Minimizar automaticamente a janela de categorias` após seleção.
  - `Lembrar o tamanho da janela de categorias`.
- **Comportamento de reprodução**:
  - `Falar a frase imediatamente ao selecionar` (reprodução automática ao clicar).
  - `Parar reprodução anterior` — para automaticamente qualquer reprodução em curso antes de iniciar uma nova frase.
  - `Aumentar o volume do SimonSays ao reproduzir` — aumenta o volume mestre do sistema enquanto fala, restaurando-o quando a reprodução termina.
  - `Reduzir temporariamente outros áudios ao reproduzir` — reduz o volume de todas as outras aplicações enquanto fala, restaurando-o quando a reprodução termina.
- `OK` guarda as alterações; `Cancelar` descarta.

## Menu do ícone da área de notificação
- `Mostrar` / `Ocultar` interface principal.
- `Definições`.
- `Sobre` mostra versão e créditos.
- `Web` abre a página do projeto (se configurada).
- `Comentários` abre o [formulário de feedback](https://forms.gle/KMbpEDYmwnFJUhgy8) no navegador padrão.
- `Sair` fecha o SimonSays.

Pode mover a janela pop-up da área de notificação e outros diálogos arrastando a barra de título; redimensione a janela de Categorias arrastando as bordas, e ative `Lembrar o tamanho da janela de categorias` nas Definições para manter o tamanho.

## Misturar fala e sons
- Use o delimitador `♫` (`SOUND_NOTE_DELIMITER`) para inserir nomes de ficheiros de áudio no texto:
  - Exemplo: `Olá ♫notification.wav♫, por favor aguarde.`
  - O texto fora dos delimitadores é falado; o texto entre delimitadores é tratado como ficheiro de som e reproduzido em linha.
- Formatos de áudio suportados: `.wav`, `.mid`, `.midi`, `.mp3`.
- Use caminhos absolutos ou relativos acessíveis ao processo. Emparelhe delimitadores para evitar que o texto final seja tratado como áudio.

## Importar e exportar categorias
- Exportar: pode exportar todas as categorias ou apenas a selecionada quando solicitado. As mensagens de sucesso/erro aparecem no idioma selecionado (atalho: `F10`).
- Importar: escolha um ficheiro; se a categoria existir, será perguntado se deseja sobrescrever. As mensagens de sucesso/erro aparecem no idioma selecionado (atalho: `F9`).

## Resolução de problemas
- **Falha na inicialização da voz**: verifique se o SAPI está instalado e se existe uma voz compatível.
- **Sem reprodução de sons**: confirme caminho/extensão e delimitadores `♫` emparelhados.
- **Erros de importação/exportação**: verifique permissões e localização do disco; tente novamente.
- **Idioma da UI não aplicado**: reabra as Definições e confirme a seleção de idioma; certifique-se de que existem cadeias de localização para o idioma escolhido.

## Sobre
- O diálogo `Sobre` mostra versão, descrição e direitos de autor.
