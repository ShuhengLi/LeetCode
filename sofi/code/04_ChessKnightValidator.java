import java.util.ArrayList;
import java.util.List;

/**
 * SoFi Tech Screen - 真题（Logical & Maintainability Round）2024/2025
 *
 * 题目：Chess Board Move Validator
 * ─────────────────────────────────────────────────────────────────
 * 设计一个国际象棋棋盘系统，验证不同棋子的走法是否合法。
 * 要求至少实现 Knight（马）的走法验证。
 *
 * 背景（国际象棋规则）：
 *   棋盘：8×8，行列坐标均为 0-7。
 *   Knight（马）：走"L"形
 *     - 横/纵方向移动 2 格，另一方向移动 1 格
 *     - 共 8 种可能移动方向
 *     - 可以"跳过"其他棋子（不受阻挡）
 *   Bishop（象）：沿对角线任意格数移动，不能穿越其他棋子
 *   Rook（车）：沿行/列任意格数移动，不能穿越其他棋子
 *   Queen（皇后）：= Bishop + Rook
 *   King（国王）：任意方向移动一格
 *
 * 这道题考察的是 OOP 设计能力：
 *   - 合理的类层次（抽象基类 / 接口）
 *   - 可扩展性（新增棋子类型不影响已有代码）
 *   - 代码可维护性
 * ─────────────────────────────────────────────────────────────────
 *
 * 设计思路（Senior/Staff 考察重点）：
 *   1. 抽象 Piece 基类，定义 isValidMove 抽象方法
 *   2. 每种棋子继承 Piece，实现各自的走法规则
 *   3. ChessBoard 持有棋盘状态，提供位置合法性校验
 *   4. 开闭原则：新增棋子只需新建子类，不改现有代码
 */
public class ChessKnightValidator {

    // ─── 棋盘坐标 ────────────────────────────────────────────────
    static class Position {
        final int row, col;

        Position(int row, int col) {
            this.row = row;
            this.col = col;
        }

        boolean isValid() {
            return row >= 0 && row < 8 && col >= 0 && col < 8;
        }

        @Override
        public String toString() {
            return "(" + row + "," + col + ")";
        }
    }

    // ─── 棋子类型枚举 ────────────────────────────────────────────
    enum PieceType { KNIGHT, BISHOP, ROOK, QUEEN, KING, PAWN }

    // ─── 抽象棋子基类 ────────────────────────────────────────────
    abstract static class Piece {
        protected final PieceType type;
        protected Position position;

        Piece(PieceType type, Position position) {
            this.type = type;
            this.position = position;
        }

        /**
         * 验证从当前位置移动到 target 是否合法。
         * @param target   目标位置
         * @param board    棋盘（用于检查路径上是否有障碍）
         * @return true 如果走法合法
         */
        abstract boolean isValidMove(Position target, ChessBoard board);

        /**
         * 基础校验：目标位置在棋盘内，且不是起始位置。
         */
        protected boolean basicCheck(Position target) {
            return target.isValid()
                && !(target.row == position.row && target.col == position.col);
        }

        PieceType getType() { return type; }
        Position getPosition() { return position; }
    }

    // ─── Knight（马）实现 ─────────────────────────────────────────
    static class Knight extends Piece {
        // 马的 8 个 L 形跳法偏移量
        private static final int[][] MOVES = {
            {-2, -1}, {-2, +1},
            {-1, -2}, {-1, +2},
            {+1, -2}, {+1, +2},
            {+2, -1}, {+2, +1}
        };

        Knight(Position position) {
            super(PieceType.KNIGHT, position);
        }

        @Override
        boolean isValidMove(Position target, ChessBoard board) {
            if (!basicCheck(target)) return false;

            int dr = Math.abs(target.row - position.row);
            int dc = Math.abs(target.col - position.col);
            // 马走法：一维差2、另一维差1
            return (dr == 2 && dc == 1) || (dr == 1 && dc == 2);
            // 马可以跳过其他棋子，无需检查路径
        }

        List<Position> getAllValidMoves(ChessBoard board) {
            List<Position> validMoves = new ArrayList<>();
            for (int[] delta : MOVES) {
                Position target = new Position(
                    position.row + delta[0],
                    position.col + delta[1]
                );
                if (isValidMove(target, board)) {
                    validMoves.add(target);
                }
            }
            return validMoves;
        }
    }

    // ─── Bishop（象）实现 ─────────────────────────────────────────
    static class Bishop extends Piece {
        Bishop(Position position) {
            super(PieceType.BISHOP, position);
        }

        @Override
        boolean isValidMove(Position target, ChessBoard board) {
            if (!basicCheck(target)) return false;

            int dr = Math.abs(target.row - position.row);
            int dc = Math.abs(target.col - position.col);
            if (dr != dc) return false;  // 必须是对角线

            // 检查路径上是否有障碍（不含起点和终点）
            return !board.isPathBlockedDiagonal(position, target);
        }
    }

    // ─── Rook（车）实现 ───────────────────────────────────────────
    static class Rook extends Piece {
        Rook(Position position) {
            super(PieceType.ROOK, position);
        }

        @Override
        boolean isValidMove(Position target, ChessBoard board) {
            if (!basicCheck(target)) return false;

            // 必须同行或同列
            if (target.row != position.row && target.col != position.col) return false;

            return !board.isPathBlockedStraight(position, target);
        }
    }

    // ─── King（国王）实现 ─────────────────────────────────────────
    static class King extends Piece {
        King(Position position) {
            super(PieceType.KING, position);
        }

        @Override
        boolean isValidMove(Position target, ChessBoard board) {
            if (!basicCheck(target)) return false;

            int dr = Math.abs(target.row - position.row);
            int dc = Math.abs(target.col - position.col);
            // 国王每次只能移动一格（8个方向）
            return dr <= 1 && dc <= 1;
        }
    }

    // ─── 棋盘类 ───────────────────────────────────────────────────
    static class ChessBoard {
        // 简化：二维数组记录每个格子是否被占用（null = 空）
        private final Piece[][] grid = new Piece[8][8];

        void placePiece(Piece piece) {
            Position pos = piece.getPosition();
            grid[pos.row][pos.col] = piece;
        }

        boolean isOccupied(Position pos) {
            return grid[pos.row][pos.col] != null;
        }

        /**
         * 检查直线路径（行/列）上是否有障碍（不含起点和终点）
         */
        boolean isPathBlockedStraight(Position from, Position to) {
            int rowStep = Integer.signum(to.row - from.row);
            int colStep = Integer.signum(to.col - from.col);

            int r = from.row + rowStep;
            int c = from.col + colStep;

            while (r != to.row || c != to.col) {
                if (grid[r][c] != null) return true;
                r += rowStep;
                c += colStep;
            }
            return false;
        }

        /**
         * 检查对角线路径上是否有障碍（不含起点和终点）
         */
        boolean isPathBlockedDiagonal(Position from, Position to) {
            int rowStep = Integer.signum(to.row - from.row);
            int colStep = Integer.signum(to.col - from.col);

            int r = from.row + rowStep;
            int c = from.col + colStep;

            while (r != to.row || c != to.col) {
                if (grid[r][c] != null) return true;
                r += rowStep;
                c += colStep;
            }
            return false;
        }
    }

    // ─── 测试 ────────────────────────────────────────────────────
    public static void main(String[] args) {
        ChessBoard board = new ChessBoard();

        // 测试 Knight
        Knight knight = new Knight(new Position(4, 4));
        board.placePiece(knight);

        System.out.println("=== Knight at (4,4) ===");
        System.out.println("Move to (2,3): " + knight.isValidMove(new Position(2, 3), board)); // true
        System.out.println("Move to (2,5): " + knight.isValidMove(new Position(2, 5), board)); // true
        System.out.println("Move to (3,3): " + knight.isValidMove(new Position(3, 3), board)); // false (diagonal)
        System.out.println("Move to (6,5): " + knight.isValidMove(new Position(6, 5), board)); // true
        System.out.println("All valid moves: " + knight.getAllValidMoves(board));

        // 测试 King
        King king = new King(new Position(0, 4));
        board.placePiece(king);

        System.out.println("\n=== King at (0,4) ===");
        System.out.println("Move to (1,4): " + king.isValidMove(new Position(1, 4), board)); // true
        System.out.println("Move to (0,3): " + king.isValidMove(new Position(0, 3), board)); // true
        System.out.println("Move to (2,4): " + king.isValidMove(new Position(2, 4), board)); // false (2 steps)

        // 测试 Rook（测试路径阻挡）
        Rook rook = new Rook(new Position(0, 0));
        board.placePiece(rook);
        Piece blocker = new King(new Position(0, 2)); // 路径上有障碍
        board.placePiece(blocker);

        System.out.println("\n=== Rook at (0,0), blocker at (0,2) ===");
        System.out.println("Move to (0,5): " + rook.isValidMove(new Position(0, 5), board)); // false (blocked)
        System.out.println("Move to (0,1): " + rook.isValidMove(new Position(0, 1), board)); // true (no block)
    }
}

/*
 * ═══════════════════════════════════════════════════════════════
 * 面试官 Follow-up & 考察点（Senior/Staff 重点）：
 * ═══════════════════════════════════════════════════════════════
 *
 * Q: 如何扩展支持新棋子（例如 Pawn 兵）？
 * A: 新建 Pawn extends Piece，实现 isValidMove（兵只能前进，特殊规则：
 *    首次可走两格、吃子时斜走），无需修改任何现有代码 → 开闭原则。
 *
 * Q: 如何支持吃子验证（目标格有己方棋子不能走）？
 * A: 在 isValidMove 中额外检查 board.isOccupied(target) 时是否是己方棋子；
 *    需要在棋子上加 color 属性（WHITE/BLACK）。
 *
 * Q: 为什么用抽象类而不是接口？
 * A: 抽象类可以提供公共的 basicCheck 实现（避免重复代码），
 *    同时通过 abstract isValidMove 强制子类实现各自逻辑。
 *    如果需要多继承语义，则改成接口 + default method。
 *
 * Q: 如何测试这个系统？
 * A: 单元测试：对每种棋子测试合法走法、非法走法、边界走法、路径阻挡；
 *    参数化测试（JUnit 5 @ParameterizedTest）可减少重复测试代码。
 *
 * Q: 这道题属于 LLD（Low-Level Design），Senior 应能谈到：
 *   - SOLID 原则（单一职责、开闭原则）
 *   - 策略模式（isValidMove 即策略）
 *   - 如何序列化/反序列化棋盘状态（用于保存对局）
 */
