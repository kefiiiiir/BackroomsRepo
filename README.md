# StealRooms (BackroomsRepo)

`BackroomsRepo` — C++ проект на Unreal Engine 5.3 в жанре first-person, с упором на интеракции с объектами, базовые survival-механики и локализованный аудиоконтент.

## Что уже реализовано

- Персонаж от первого лица на Enhanced Input.
- Система взаимодействия с физическими объектами (`Grab`/`UnGrab`).
- Механики передвижения: спринт, приседание, стамина.
- Простой инвентарь (`ItemID -> количество`).
- Включаемый фонарик.
- Система разблокировки предметов и UI-виджет награды.
- Локализованный звук (поддержка языков через `GameInstance` и `LocalizedAudioComponent`).
- Базовая логика спавна монстра по условиям в персонаже.

## Технологии и требования

- Unreal Engine `5.3`.
- C++ модуль `BackroomsRepo`.
- Ключевые зависимости модуля:
  - `Core`
  - `CoreUObject`
  - `Engine`
  - `InputCore`
  - `EnhancedInput`
  - `UMG`
  - `PhysicsCore`
  - `Niagara`
  - `NavigationSystem`

## Структура проекта

- `BackroomsRepo.uproject` — конфигурация проекта, версия движка, плагины.
- `Source/BackroomsRepo` — основной C++ код игры.
  - `BackroomsRepoCharacter.*` — логика игрока, инвентарь, фонарик, стамина, спавн монстра.
  - `Public/GrabComponent.*` и `Private/GrabComponent.*` — захват/удержание объектов.
  - `Public/GrababbleObject.*` и `Private/GrababbleObject.*` — интерактивные физические предметы.
  - `Public/LocalizedAudio/*` и `Private/LocalizedAudioComponent.cpp` — локализованный звук.
  - `Public/DefaultGameInstance.*` — глобальное состояние (в т.ч. текущий язык).
  - `Public/ItemData.*` — структура строки DataTable для предметов.
- `Config` — настройки карт, режимов игры, упаковки, ввода и локализации.

## Запуск в редакторе

1. Откройте `BackroomsRepo.uproject` в Unreal Engine 5.3.
2. При необходимости сгенерируйте project files (через контекстное меню `.uproject`).
3. Откройте решение в Visual Studio / Rider и соберите:
   - `BackroomsRepo` (Game target)
   - `BackroomsRepoEditor` (Editor target)
4. Запустите проект через Unreal Editor.

## Карты и точки входа

По текущим конфигам:

- `EditorStartupMap`: `/Game/MAIN/MP/Map.Map`
- `GameDefaultMap`: `/Game/MAIN/ASSETS/WB/Menu.Menu`
- `GlobalDefaultGameMode`: `BP_FirstPersonGameMode` (Blueprint)
- `GameInstanceClass`: `/Script/BackroomsRepo.DefaultGameInstance`
- `MapsToCook`:
  - `/Game/MAIN/MP/Map`
  - `/Game/MAIN/ASSETS/WB/Menu`

## Плагины

В `uproject` включены:

- `ModelingToolsEditorMode` (для Editor)
- `Water`
- `WaterExtras`

## Локализация

- В упаковке добавлены культуры: `en`, `ru-RU`.
- Текущий язык хранится в `UDefaultGameInstance::CurrentLanguage`.
- Компонент `ULocalizedAudioComponent` выбирает звук по ключу и текущему языку.

## Сборка и упаковка

Базовые параметры упаковки в `Config/DefaultGame.ini`:

- `Build=IfProjectHasCode`
- `BuildConfiguration=Shipping`
- сжатие пакетов через `Oodle` (`Kraken`)
- явный список карт для cook задан через `MapsToCook`

## Замечания по репозиторию

- В репозитории присутствуют исходники и конфиги.
- Ассеты Unreal (`Content/*.uasset`, `*.umap`) в текущем состоянии не обнаружены как файлы в рабочем дереве.
- Из-за этого часть контента (карты/блюпринты/виджеты), упомянутая в `.ini`, может находиться вне текущего набора файлов.

## Игнорируемые артефакты

`.gitignore` уже исключает стандартные UE-директории:

- `Binaries/`
- `Build/`
- `DerivedDataCache/`
- `Intermediate/`
- `Saved/`