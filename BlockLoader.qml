import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0

/**
 * The design is taken from here: https://dribbble.com/shots/3250272-Animated-Loader-Principle-Freebie
 **/
Item {

    // ----- Public Properties ----- //

    property int spacing: 2
    property int horizontalBlockCount: 4
    property int verticalBlockCount: 3

    id: root

    Repeater {
        id: repeater
        model: root.horizontalBlockCount
        delegate: Component {
            Column {
                // ----- Private Properties ----- //
                property int _columnIndex: index

                id: column
                spacing: root.spacing
                x: root._getRectSize().width * index + ((index - 1) * root.spacing)

                Repeater {
                    // ----- Private Properties ----- //
                    property alias _columnIndex: column._columnIndex

                    id: blockRepeater
                    model: root.verticalBlockCount
                    delegate: Component {
                        Rectangle {
                            id: rect
                            width: root._getRectSize().width
                            height: root._getRectSize().height
                            radius: 2
                            smooth: true
                            transformOrigin: Item.BottomRight

                            SequentialAnimation {
                                id: rotationAnimation
                                loops: Animation.Infinite

                                RotationAnimation {
                                    target: rect
                                    duration: 1000
                                    from: 0
                                    to: 90
                                    easing.type: Easing.InOutQuint
                                }

                                PauseAnimation { duration: blockRepeater._columnIndex * 300 + 300 * blockRepeater._columnIndex }

                                RotationAnimation {
                                    target: rect
                                    duration: 1000
                                    from: 90
                                    to: 0
                                    easing.type: Easing.InOutQuint
                                }

                                PauseAnimation { duration: (blockRepeater.model - blockRepeater._columnIndex) * 300 + 300 * (blockRepeater.model - blockRepeater._columnIndex) }
                            }

                            function startAnimation() {
                                if (rotationAnimation.running === false) {
                                    rotationAnimation.start();
                                }
                            }

                            function stopAniamtion(){
                                if(rotationAnimation.running){
                                    rotationAnimation.stop();
                                }
                            }
                        }
                    }
                }

                // ----- Public Functions ----- //

                function startAnimation() {
                    var count = blockRepeater.model;
                    for (var index = 0; index < count; index++) {
                        blockRepeater.itemAt(index).startAnimation();
                    }
                }

                function stopAniamtion() {
                    var count = blockRepeater.model;
                    for (var index = 0; index < count; index++) {
                        blockRepeater.itemAt(index).stopAniamtion();
                    }
                }
            }
        }
    }

    Timer {
        // ----- Private Properties ----- //
        property int _blockIndex: root.horizontalBlockCount - 1
        id:animationTimer
        interval: 300
        repeat: true
        running: false
        onTriggered: {
            if (_blockIndex === -1) {
                stop();
            }
            else {
                repeater.itemAt(_blockIndex).startAnimation();
                _blockIndex--;
            }
        }
//        Component.onCompleted: start()
    }

    // ----- Private Functions ----- //

    function _getRectSize() {
        var w = root.width / root.horizontalBlockCount;
        return Qt.size(w, w);
    }

    function startAnimation(){
        animationTimer.start()
    }

}
